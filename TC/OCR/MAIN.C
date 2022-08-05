#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "split.h"
#include "blobify.h"
#include "net.h"
#include "pattern.h"
#include "myutilities.h"
#include "xshowimage.h"
#include "utilities.h"

main(int argc, char **argv)
{
   FILE *tf;
   net_t *net;
   train_set_t train_set;
   int count, iterate, ndx, line;
   float rate, reduction, error, initial;
   int holdchoice;
   char fixfile[50];
   blob_type *thelist,*tmp_list;
   image_type *myimage_tmp;
   int tmpcount=0;
   val_t backerror;
   int counter;

   
  printf("Do you want to (1) train the net or (2) create train file\n");
  scanf("%d",&holdchoice);

  if (holdchoice == 1) /*beginning of case 1*/
  {
   if (argc != 4)
     bye("Usage for training net: testnet netfile activationfile trainfile\n");

   net = init_net(argv[1], argv[2]);
   tf = chk_fopen(argv[3], "r");
   if (fscanf(tf, "%f %f %d %d", &rate, &reduction, &iterate, &count) != 4)
      bye("Error reading initial data from file %s\n", argv[3]);
   train_set.inputs = (pattern_t *) chk_malloc(count * sizeof(pattern_t));
   train_set.outputs = (pattern_t *) chk_malloc(count * sizeof(pattern_t));
   train_set.count = count;
   for (ndx = 0; ndx < count; ndx++) {
      train_set.inputs[ndx].data = 
       (val_t *) chk_malloc(net->inputs * sizeof(val_t));
      train_set.outputs[ndx].data = 
       (val_t *) chk_malloc(net->outputs * sizeof(val_t));
      train_set.inputs[ndx].count = net->inputs;
      train_set.outputs[ndx].count = net->outputs;
      for (line = 0; line < net->inputs; line++)
         if (fscanf(tf, "%f", train_set.inputs[ndx].data + line) != 1)
            bye("Error reading input line %d in pattern %d\n", line, ndx);
      for (line = 0; line < net->outputs; line++)
         if (fscanf(tf, "%f", train_set.outputs[ndx].data + line) != 1)
            bye("Error reading output line %d in pattern %d\n", line, ndx);
   }
   error = initial = train(net, &train_set, rate);
   backerror=error;
   counter=0;
   while (error > initial*reduction && --iterate > 0)
   {
      error = train(net, &train_set, rate);
      if ((backerror > 1.0) && (backerror < error))
	rate = rate * 0.7;
      counter++;
      if ((counter % 5) == 0)
      {
	system("rm netfile.out");
	dump_net("netfile.out",net);
      }
   } 
   dump_net("netfile.out", net);
  } /*end of case 1 */
  else if(holdchoice == 2) /*Beginn case 2 */
  {
    printf("Please enter the file from which the train set will be created:\n");
    scanf("%s",fixfile);
    thelist = NULL;
    myimage_tmp = read_pgm_image(fixfile);
    myimage_tmp = split_image(myimage_tmp);
    blobify(myimage_tmp,255,0,&(thelist));

    tmp_list = thelist;

/*  debugging loop for blob testing
    while (tmp_list != NULL)
    {
       x_show_image(tmp_list->image,"Testing");
       printf("count: %d yloc: %d xloc: %d\n",tmpcount,tmp_list->y_loc,tmp_list->x_loc);
       tmp_list = tmp_list->next;
       tmpcount++;
    }
*/

    make_train_file(thelist);
  } /*end case 2*/
}
