
SUB_DIR = example
 
$(SUB_DIR):
	@+make -C $@    

.PHONY: $(SUB_DIR)
