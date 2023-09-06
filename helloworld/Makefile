TARGET=helloworld  
LDFLAGS= -lpthread  -lql_mcm_api -lql_ril -lql_misc
 
CUR_SOURCE=${wildcard *.c}
CUR_OBJS=${patsubst %.c, %.o, $(CUR_SOURCE)}

all: $(TARGET)

$(TARGET): $(CUR_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(CUR_OBJS) $(TARGET)