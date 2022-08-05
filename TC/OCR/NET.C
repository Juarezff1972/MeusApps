#include <stdio.h>
#include "mymacros.h"
#include "net.h"
#include "myutilities.h"

val_t get_derivative(val_t *data,val_t number)
{

 val_t sumval;

 sumval = number/RES_VAL;

 if (sumval > MAX_VAL)
   sumval = MAX_VAL;
 else if (sumval < MIN_VAL)
   sumval = MIN_VAL;

 return data[(int)sumval];

}

static void set_activation(char *act_file, net_t *net)
{
   int i;
   FILE *fp;

   fp = chk_fopen(act_file, "r");
   net->activate =
    (val_t *) chk_malloc((MAX_VAL - MIN_VAL + 1) * sizeof(val_t));
   net->derivative =
    (val_t *) chk_malloc((MAX_VAL - MIN_VAL + 1) * sizeof(val_t));
   net->activate -= MIN_VAL;
   net->derivative -= MIN_VAL;
   for (i = MIN_VAL; i <= MAX_VAL; i++) 
      if (fscanf(fp, "%f %f", net->activate + i, net->derivative + i) != 2)
         bye("Error reading activation values on %dth entry\n", i);
   fclose(fp);
}

net_t *init_net(char *net_file, char *act_file)
{
   FILE *fp;
   int nodes, inputs, outputs, this_node, in_node;
   net_t *net;
   node_t *n_list;
   float weight;
   link_t *link, *new_link;

   fp = chk_fopen(net_file, "r");
   if (fscanf(fp, "%d %d %d", &nodes, &inputs, &outputs) != 3) 
      bye("Error reading initial line in %s\n", net_file);
   if (nodes < inputs + outputs || inputs <= 0 || outputs <= 0) 
      bye("Invalid numbers for initial data\n");

   net = get_new(net_t);
   net->nodes = n_list = (node_t *) chk_malloc((nodes+1)*sizeof(node_t));
   for (this_node = 0; this_node <= nodes; this_node++) {
      n_list[this_node].b_list = n_list[this_node].f_list = NULL;
   }
   n_list[0].value = ONE_VAL;
   set_activation(act_file, net);

   fscanf(fp, "%*[ \t\n]");
   while (fscanf(fp, "Node %d", &this_node) == 1) {
      if (this_node <= inputs || this_node > nodes) 
         bye("No inputs allowed to node %d\n", this_node);
      while (fscanf(fp, "%d %f", &in_node, &weight) == 2) {
         if (in_node < 0 || in_node > nodes - outputs) 
            bye("Node %d may not feed other nodes\n", in_node);
         if (in_node >= this_node)
            fprintf(stderr, "Warning: potential backward link from %d to %d\n",
             in_node, this_node);
         for (link=n_list[this_node].b_list; link != NULL; link = link->b_next)
            if (link->b_ptr == n_list+in_node)
               fprintf(stderr, "Node %d feeds multiple times to node %d\n",
                in_node, this_node);
         new_link = get_new(link_t);
         new_link->weight = weight;
         new_link->momentum = new_link->error = ZERO_VAL;
         new_link->b_ptr = n_list+in_node;
         new_link->f_ptr = n_list+this_node;
         new_link->f_next = n_list[in_node].f_list;
         n_list[in_node].f_list = new_link;
         new_link->b_next = n_list[this_node].b_list;
         n_list[this_node].b_list = new_link;
      }
   }
   if (!feof(fp))
      bye ("Failed to reach eof in net file %s\n", net_file);
   net->inputs = inputs;
   net->outputs = outputs;
   net->all = nodes;
   net->nodes++;
   fclose(fp);
   return net;
}

void feed_forward(net_t *net, pattern_t *input)
{

   int i,numinputs,numtotnodes,loopval;
   node_t *topnode;
   node_t *holdnode;
   link_t *holdlink;
   val_t sumval=0.0;
   val_t *fdata;

   /* Setup the dummy layer of inputs */

   fdata = input->data;
   holdnode = net->nodes;
   numinputs = input->count;

   holdnode[-1].value = ONE_VAL;

   for(i=0;i < numinputs;i++)
   {
     holdnode->value = *(fdata);
     fdata++;  
     holdnode++;
   }   


  /* Do the feed forward for real */

  numinputs = net->inputs;
  numtotnodes = net->all;
  topnode = holdnode;

  for(i=numinputs;i < numtotnodes;i++)
  {
   holdlink = topnode->b_list;
   sumval=0.0;
   while (holdlink != NULL)
   {
     holdnode = holdlink->b_ptr;
     sumval += (holdnode->value * holdlink->weight);
     holdlink = holdlink->b_next;
   }
   topnode->sum = sumval;

   topnode->value = get_derivative(net->activate,sumval);
   /* printf("Node:%d to %f \n",i,(topnode->value));  */
   topnode++;
  }
/*  printf("Leaving feed\n"); */
}

pattern_t *get_output(net_t *net)
{
   pattern_t *output;
   node_t *node;
   int count;

   output = get_new(pattern_t);
   output->data = (val_t *) chk_malloc(net->outputs*sizeof(val_t));
   output->count = net->outputs;
   for (node = net->nodes + net->all - net->outputs, count = 0; 
    count < net->outputs; node++, count++)
      output->data[count] = node->value;
   return output;
}

void back_propagate(net_t *net, pattern_t *model)
{
  int i,loopval;
  node_t *lastnode;
  link_t *holdlink;
  val_t *dataval;
  val_t holdval;
  val_t sumval;

  /* Setup and execute back prop for last layer */

  loopval = net->outputs;
  lastnode = net->nodes + (net->all - loopval);
  dataval = model->data;

  for(i=0;i < loopval;i++)
  {
     holdval = get_derivative(net->derivative,lastnode->sum);
     lastnode->derivative = (lastnode->value - (*dataval)) * holdval;
     dataval++;
     lastnode++;
  }

  loopval = (net->all - net->outputs) -1;
  lastnode = net->nodes + loopval;

  for(i=loopval;i >= -1;i--)
  {
    sumval = 0.0;
    holdlink = lastnode->f_list;

    while (holdlink != NULL)
    {
       holdval = holdlink->f_ptr->derivative;
       holdlink->derivative = lastnode->value * holdval;
       sumval += holdlink->weight * holdval;
       holdlink = holdlink->f_next;
    }

    lastnode->derivative = sumval * get_derivative(net->derivative,lastnode->sum);
    lastnode--;
  }

}

void adjust(net_t *net, val_t factor)
{
  
  int i;
  int loopval;
  link_t *holdlink;
  node_t *startnode;

  /* printf("Entering adjust\n") */;
  startnode = net->nodes + net->inputs;
  loopval = net->all - net->inputs; 

  for(i=0;i < loopval;i++)
  {
    holdlink = startnode->b_list;

    while( holdlink != NULL)
    {
       holdlink->weight += -holdlink->derivative * factor - holdlink->momentum;
       holdlink->momentum += holdlink->derivative * factor;
       holdlink->momentum *=0.5;
       holdlink = holdlink->b_next;
    }

    startnode++;
  }
}

float train(net_t *net, train_set_t *set, float rate)
{
   int ndx, line;
   val_t error;
   pattern_t *output;
   float all_error;

   all_error = 0.0;
   for (ndx = 0; ndx < set->count; ndx++) {
      feed_forward(net, set->inputs + ndx);
      output = get_output(net);
      back_propagate(net, set->outputs + ndx);
      adjust(net, rate);

      error = ZERO_VAL;
      for (line = 0; line < output->count; line++)
         error += sqr((output->data[line] - set->outputs[ndx].data[line]));
      free(output->data);
      free(output);
      all_error += error/2.0;
   }
fprintf(stderr, "Current error value: %f\n", all_error);
   return all_error;
}

#define MAX_COL 5

void dump_net(char *filename, net_t *net)
{
   FILE *fp;
   int col;
   node_t *node;
   link_t *link;

   fp = chk_fopen(filename, "w");

   fprintf(fp, "%d %d %d\n\n", net->all, net->inputs, net->outputs);

   for (node = net->nodes + net->inputs; node < net->nodes+net->all; node++) {
      fprintf(fp, "Node %3d    ", node - net->nodes + 1);
      col = 0;
      for (link = node->b_list; link != NULL; link = link->b_next) {
         if (col > 0 && col++ % MAX_COL == 0)
            fprintf(fp, "\n          ");
         fprintf(fp, " %3d %7.4f", link->b_ptr - net->nodes + 1, link->weight);
      }
      fprintf(fp, "\n");
   }
}
