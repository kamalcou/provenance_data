#!/usr/bin/python

from __future__ import print_function
import json
from bcc import BPF
from ctypes import c_uint32
import time,threading
import collections
import os
import pwd
from datetime import datetime


import sys
import subprocess

sys.path.append('/usr/local/etc/filemonitor/cli.py')
import cli

BPF_C_PROG = "filemonitor.c"


# filepath_folder="/home/exouser/filemonitor/files"
filepath_folder="/media/volume/sdb/file"


# initialize global variables
def init():
    global BPF_C_PROG
    try:
        if(open("/usr/local/etc/filemonitor/filemonitor.c")):
            BPF_C_PROG = "/usr/local/etc/filemonitor/filemonitor.c"
    except:
        pass


def uid_to_username(uid):
    try:
        user_info = pwd.getpwuid(uid)
        username = user_info.pw_name
        return username
    except KeyError:
        return None
    
files_list=[]   
# update_inodemap function takes BPFHASH inodemap, config file as arguments
# reads config file, finds the inode and updates inodemap
def update_inodemap(inodemap, config_file):
    #if not config_file:
    #    raise FileNotFoundError

    #file = open(config_file, 'r')
    #filepaths = file.readlines()
    filepaths=[]
    
    
    
    for root, dirs, files in os.walk(os.path.abspath(filepath_folder)):
        for file in files:
            #print(os.path.join(root, file))
            filepaths.append(os.path.join(root, file))
            files_list.append(file)
            
            # print(file)
    #print(type(filepaths))
    for filepath in filepaths:
        inode_id = get_inode_from_filepath(filepath.strip())
        # print(os.stat(filepath).st_ino, filepath)
        # print(filepath)
        
        if inode_id != "":
            inodemap[c_uint32(int(inode_id))] = c_uint32(int(inode_id))



mod_dict = collections.defaultdict(dict)
file_dict=collections.defaultdict(dict)
def updates_dict(filename):
    global mod_dict
    print("Before updates dict:",mod_dict)
    #if(filename in mod_dict):
    	#mod_dict.pop(filename)
    mod_dict.clear()
    print (" Updated on",time.ctime())
    
#check the mode of the file
def check_mode(filename, mode,comm,pid,CPU,uid,timestamp):
	
    global mod_dict
	#print(comm)
	#updates_dict()
	#print(mod_dict)
	#if(filename in mod_dict):
	#	print("test  ----####",mod_dict[filename]['program'])
	
    username=uid_to_username(uid)
    
	#if((filename in mod_dict is None or filename[0] in mod_dict is None)):
    if((filename not in mod_dict)):
        
        mod_dict.update({filename:{'program':comm,'mode':mode,'pid':pid,'username':username,'timestamp':timestamp}})
        #mod_dict[filename][comm][pid]=mode
        
        print("track event")
        # print(mod_dict)
        return 1
    else:
        
        if(mod_dict[filename]['program']==comm and mod_dict[filename]['mode']==mode and mod_dict[filename]['pid']==pid and mod_dict[filename]['username']==username ):
            # print ("No change")
            return 0
        # elif(mod_dict[filename]['timestamp']!=timestamp):
        else: 
            mod_dict.update({filename:{'program':comm,'mode':mode,'pid':pid,'username':username,'timestamp':timestamp}})
            print ("change mode")
            # print(mod_dict)
            return 1

        
        
     	
            
def submitTransaction(filename, mode,pid,username,timestamp,cpu,operation):
    
    cmd = 'node /home/exouser/fab/fabric-samples/fabcar/javascript/query.js'
    cmd+=" "+"addRecord"
    cmd+=" "+filename
    cmd+=" "+mode
    cmd+=" "+str(pid)
    cmd+=" "+username
    cmd+=" "+str(timestamp)
    cmd+=" "+str(cpu)
    cmd+=" "+operation

    
    # print(cmd)
    os.system(cmd)
    

    
    

# main function reads args and attaches bpf program
# prints output of bpf events
def main():
    args = cli.parser.parse_args()
    noflags = cli.noflags(args)
    
    
    for root, dirs, files in os.walk(os.path.abspath(filepath_folder)):
        for file in files:
            cmd = 'node /home/exouser/fab/fabric-samples/fabcar/javascript/invoke.js'
            
            cmd+=" "+"addNewFile"
            cmd+=" "+file
            file_path = filepath_folder+'/'+file
            
            # Get file statistics
            file_stat = os.stat(file_path)
            creation_time = int(os.path.getctime(file_path))


            # Get the owner's user ID
            owner_uid = file_stat.st_uid

            # Use pwd module to get owner's information
            owner_info = pwd.getpwuid(owner_uid)

            # Extract owner's username
            owner_username = owner_info.pw_name
            cmd+=" "+owner_username
            # Get the creation time of the file
            # creation_time = time.ctime(file_stat.st_ctime)
            cmd+=" "+str(creation_time)
            print(cmd)
            os.system(cmd)
    
    
    try:
        # initialize bpf program
        global BPF_C_PROG
        b = BPF(src_file = BPF_C_PROG)

        # update inodemap
        update_inodemap(b["inodemap"], args.file)
        
        # attach probes
        if noflags or args.read:
            b.attach_kprobe(event="vfs_read", fn_name="trace_read")
        if noflags or args.write:
            b.attach_kprobe(event="vfs_write", fn_name="trace_write")
            
        if noflags or args.rename:
            b.attach_kprobe(event="vfs_rename", fn_name="trace_rename")
        if noflags or args.create:
            b.attach_kprobe(event="security_inode_create", fn_name="trace_create")
        if noflags or args.delete:
            b.attach_kprobe(event="vfs_unlink", fn_name="trace_delete")
        
        
        # header
        print("%-6s %-4s %-4s %-10s %-10s %-10s %-4s" % ("PID", "UID", "CPU", "PROC", "FPATH", "COMM", "OPRN"))
        
        
        process_and_CPU=[]
        filename=""
        dict_filename={}
        
        def print_event(cpu, data, size):
            event = b["events"].event(data)
            global filename
            return_val=0

            #global dict
            #if((event.otype.decode('utf-8', 'replace')!='WRITE') and (event.otype.decode('utf-8', 'replace')!='RENAME') and (event.fname.decode('utf-8', 'replace')[0]!='.')):
            if(event.otype.decode('utf-8', 'replace')=='WRITE'):
                # update inodemap
               update_inodemap(b["inodemap"], args.file)
            
            
            
            #print(event.fname.decode('utf-8', 'replace'),"\n",event.otype.decode('utf-8', 'replace'))
            if(event.fname.decode('utf-8', 'replace')[0]=='.'):
                dict_filename.update({event.fname.decode('utf-8', 'replace'):filename})
                filename=dict_filename[event.fname.decode('utf-8', 'replace')]
            else:
            	filename=event.fname.decode('utf-8', 'replace')
            
            
            #print(dict_filename)
            	
            #threading.Timer(30,updates_dict,args=(filename,)).start()
            #else:
             #   dict["event.fname.decode('utf-8', 'replace')"]=filename
            # if(cpu==0):
            mode=event.otype.decode('utf-8', 'replace')
            curr_dt = datetime.now()
            timestamp = int(round(curr_dt.timestamp()))
            
            
            if(event.otype.decode('utf-8', 'replace')!='RENAME'):
                    return_val=check_mode(filename, mode,event.comm.decode('utf-8', 'replace'),event.pid,cpu,event.uid,timestamp)
                   
            # elif(cpu==1 and event.otype.decode('utf-8', 'replace')=='WRITE'):
            #     return_val=check_mode(filename, event.otype.decode('utf-8', 'replace'),event.comm.decode('utf-8', 'replace'),event.pid,cpu,event.uid,timestamp)
            # else:
            #     return_val=0    
            
            #print(return_val)
            #print("dict: ", dict["event.fname.decode('utf-8', 'replace')"])
            #count+=1
            #print ("count: ", count)
            #print("event: ", event.fname.decode('utf-8', 'replace'))
            #print("filename: ", filename)
            #if(empty(list[event.pid][event.uid])):
            #	list[event.pid][event.uid]=event.fname.decode('utf-8', 'replace');
            #print event.fname.decode('utf-8', 'replace')
            if(return_val==1):
            	#print("%-6d %-4d %-4d %-32s %-32s %-32s %-4s" % (event.pid, event.uid, cpu,
                #event.pname.decode('utf-8', 'replace'), event.fname.decode('utf-8', 'replace'),
                #event.comm.decode('utf-8', 'replace'), event.otype.decode('utf-8', 'replace')))
                print("%-6d %-4d %-4d %-10s %-10s %-10s %-4s" % (event.pid, event.uid, cpu,
                event.pname.decode('utf-8', 'replace'), filename,
                event.comm.decode('utf-8', 'replace'), event.otype.decode('utf-8', 'replace')))
                username1=uid_to_username(event.uid)
                mode1=event.otype.decode('utf-8', 'replace')
                curr_dt = datetime.now()
                timestamp1 = int(round(curr_dt.timestamp()))
                program_name=event.comm.decode('utf-8', 'replace')
                # os.system("echo 'test'")
                if((event.pid,cpu,mode) not in process_and_CPU):
                    # print("duplicate\n")
                    submitTransaction(filename, mode1,event.pid,username1,timestamp1,cpu,program_name)
                    process_and_CPU.append((event.pid,cpu,mode))
                



        b["events"].open_perf_buffer(print_event)
        while 1:
            try:
                b.perf_buffer_poll()
            except KeyboardInterrupt:
                exit(0)
    except FileNotFoundError:
        print("Exception occured, Is filepath correct?")
    except Exception as e:
        print("Exception occured, Are you root? Is BPF installed?", e)

# get_inode_from_filepath takes a filepath as argument
# and returns inode associated with that file path
def get_inode_from_filepath(filepath):
  cmd = f'ls {filepath} 2>&1 1>/dev/null && ls -i {filepath}'
  cmd += ' | awk \'{print $1}\''
  try:
    output = subprocess.check_output(cmd, shell=True)
    output = output.decode()
    return output.split('\n')[0]
  except:
      return ""

# starts program
if __name__ == "__main__":
    init()
    main()
