# change these three according to your environment, or load apropriate
# modules(1)
#HDF5="add your path to HDF5"
MPICC=h5pcc

#CPPFLAGS=-I${HDF5}/include
#LDFLAGS=-L${HDF5}/lib


# PROGRAMS= h5_ex0 h5_ex1 h5par_ex0 h5par_ex1a h5par_ex1b h5par_ex1c h5par_ex1d\
	  h5par_ex2a h5par_ex2b h5par_ex2c h5par_ex2d h5par_ex2e \
	  h5par-comparison h5par-comparison-collio h5par-comparison-collmd

PROGRAMS= experiment experiment1 append append1  file_read file_write
all: $(PROGRAMS)


file_read: file_read.c
	$(MPICC) $(CFLAGS) $(CPPFLAGS) $^ -o $@ $(LDFLAGS) -lhdf5
file_write: file_write.c
	$(MPICC) $(CFLAGS) $(CPPFLAGS) $^ -o $@ $(LDFLAGS) -lhdf5
experiment: experiment.c
	$(MPICC) $(CFLAGS) $(CPPFLAGS) $^ -o $@ $(LDFLAGS) -lhdf5
experiment1: experiment1.c
	$(MPICC) $(CFLAGS) $(CPPFLAGS) $^ -o $@ $(LDFLAGS) -lhdf5

append: append.c
	$(MPICC) $(CFLAGS) $(CPPFLAGS) $^ -o $@ $(LDFLAGS) -lhdf5
append1: append1.c
	$(MPICC) $(CFLAGS) $(CPPFLAGS) $^ -o $@ $(LDFLAGS) -lhdf5
extend2: extend2.c
	$(MPICC) $(CFLAGS) $(CPPFLAGS) $^ -o $@ $(LDFLAGS) -lhdf5
h5_ex0: h5_ex0.c
	$(MPICC) $(CFLAGS) $(CPPFLAGS) $^ -o $@ $(LDFLAGS) -lhdf5

h5_ex1: h5_ex1.c
	$(MPICC) $(CFLAGS) $(CPPFLAGS) $^ -o $@ $(LDFLAGS) -lhdf5

h5par_ex0: h5par_ex0.c
	$(MPICC) $(CFLAGS) $(CPPFLAGS) $^ -o $@ $(LDFLAGS) -lhdf5

h5par_ex1a: h5par_ex1a.c
	$(MPICC) $(CFLAGS) $(CPPFLAGS) $^ -o $@ $(LDFLAGS) -lhdf5

h5par_ex1b: h5par_ex1b.c
	$(MPICC) $(CFLAGS) $(CPPFLAGS) $^ -o $@ $(LDFLAGS) -lhdf5

h5par_ex1c: h5par_ex1c.c
	$(MPICC) $(CFLAGS) $(CPPFLAGS) $^ -o $@ $(LDFLAGS) -lhdf5

h5par_ex1d: h5par_ex1d.c
	$(MPICC) $(CFLAGS) $(CPPFLAGS) $^ -o $@ $(LDFLAGS) -lhdf5

h5par_ex2a: h5par_ex2a.c
	$(MPICC) $(CFLAGS) $(CPPFLAGS) $^ -o $@ $(LDFLAGS) -lhdf5

h5par_ex2b:  h5par_ex2b.c
	$(MPICC) $(CFLAGS) $(CPPFLAGS) $^ -o $@ $(LDFLAGS) -lhdf5

h5par_ex2c:  h5par_ex2c.c
	$(MPICC) $(CFLAGS) $(CPPFLAGS) $^ -o $@ $(LDFLAGS) -lhdf5

h5par_ex2d:  h5par_ex2d.c
	$(MPICC) $(CFLAGS) $(CPPFLAGS) $^ -o $@ $(LDFLAGS) -lhdf5

h5par_ex2e:  h5par_ex2e.c
	$(MPICC) $(CFLAGS) $(CPPFLAGS) $^ -o $@ $(LDFLAGS) -lhdf5
2d_write:  2d_write.c
	$(MPICC) $(CFLAGS) $(CPPFLAGS) $^ -o $@ $(LDFLAGS) -lhdf5

h5par-comparison:  h5par-comparison.c
	$(MPICC) $(CFLAGS) $(CPPFLAGS) $^ -o $@ $(LDFLAGS) -lhdf5
	$(MPICC) $(CFLAGS) $(CPPFLAGS) -DUSE_COLL_DATA $^ -o h5par-comparison-collio $(LDFLAGS) -lhdf5
	$(MPICC) $(CFLAGS) $(CPPFLAGS) -DUSE_COLL_DATA -DUSE_COLL_MD $^ -o h5par-comparison-collmd $(LDFLAGS) -lhdf5

clean:
	rm -f *.o *.h5 $(PROGRAMS)

