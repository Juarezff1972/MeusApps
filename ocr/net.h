#define MAX_VAL  2048
#define MIN_VAL -2048
#define RES_VAL .015625
#define ZERO_VAL 0.0
#define ONE_VAL 1.0

typedef float val_t;

typedef struct node_t {
   char *id;
   val_t derivative;
   val_t value;   
   val_t sum;
   struct link_t *b_list;
   struct link_t *f_list;
} node_t;

typedef struct link_t {
   val_t weight;
   val_t derivative;
   val_t error;
   val_t momentum;
   node_t *b_ptr;
   node_t *f_ptr;
   struct link_t *b_next;
   struct link_t *f_next;
} link_t;

typedef struct pattern_t {
   val_t *data;
   int count;
} pattern_t;

typedef struct net_t {
   node_t *nodes;
   int inputs, outputs, all;
   val_t *activate, *derivative;
} net_t;

typedef struct train_set_t {
   pattern_t *inputs;
   pattern_t *outputs;
   int count;
} train_set_t;

net_t *init_net(char *, char *);
void feed_forward(net_t *, pattern_t *);
pattern_t *get_output(net_t *);
void back_propagate(net_t *, pattern_t *);
void adjust(net_t *, val_t);
float train(net_t *, train_set_t *, float);
void dump_net(char *, net_t *);
