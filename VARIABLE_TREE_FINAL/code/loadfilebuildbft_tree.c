#include "bloomfiltertree.h"
#include "bloomfilter.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>







void build_bf_tree(DOUBLENODEBLOOMFILTERTREE *bft){
    if(bft->type == FIX && !bft->built){
        unsigned int Currentnumberofnodes = bft->leavesSize;
        unsigned int Abovenumberofnodes = countTheNumberOfNodesAboveTheseNodes(Currentnumberofnodes);
        unsigned int totalnodes = Currentnumberofnodes + Abovenumberofnodes;
        unsigned int level = (unsigned int)log2(totalnodes);
        
        
        for(int i = level; i > 0; i--){
            #ifdef LOGGING
                printf("---------------------level: %d----------------------------\n",i);
            #endif 

            int k = 0;
            char state = 0;
            for(unsigned int j = totalnodes; j > Abovenumberofnodes; j -= 2){
                // printf("totalnodes: %d\n",totalnodes);
                // printf("Abovenumberofnodes: %d\n",Abovenumberofnodes);
                if(j != totalnodes){

                    if(state == 0){
                        bft->data[Abovenumberofnodes-k] = bf_union(bft->data[j-1], bft->data[j]);
                        bft->data[Abovenumberofnodes-k]->leftparentnode = j-1;
                        bft->data[Abovenumberofnodes-k]->rightparentnode = j;
                        #ifdef LOGGING
                            printf("bft->data[%d] is generated by leftparentnode:bft->data[%d] and rightparentnode bft->data[%d] \n",Abovenumberofnodes-k,j-1,j);
                        #endif 
                        // printf("bft->data[%d]->leftparentnode = %d\n",Abovenumberofnodes-k,j-1);
                        // printf("bft->data[%d]->rightparentnode = %d\n",Abovenumberofnodes-k,j);
                        // printf("get data[%d]\n",Abovenumberofnodes-k);
                        // printfdataarray(bft->data[Abovenumberofnodes-k]);
                        k++;
                    }
                    else{
                        bft->data[Abovenumberofnodes-k] = bf_union(bft->data[j + 1], bft->data[j]);
                        bft->data[Abovenumberofnodes-k]->leftparentnode = j;
                        bft->data[Abovenumberofnodes-k]->rightparentnode = j + 1;
                        #ifdef LOGGING
                            printf("bft->data[%d] is generated by leftparentnode:bft->data[%d] and rightparentnode bft->data[%d] \n",Abovenumberofnodes-k,j,j+1);
                        #endif 
                        // printf("bft->data[%d]->leftparentnode = %d\n",Abovenumberofnodes-k,j);
                        // printf("bft->data[%d]->rightparentnode = %d\n",Abovenumberofnodes-k,j+1);                        
                        // printf("get data[%d]\n",Abovenumberofnodes-k);
                        // printfdataarray(bft->data[Abovenumberofnodes-k]);
                        k++;
                    }
                }
                else{
                    if(((j-Abovenumberofnodes)%2) == 1){
                    //     if(!(bft->data[Abovenumberofnodes] = (DOUBLENODEBLOOMFILTER *)calloc(1,sizeof(DOUBLENODEBLOOMFILTER)))){
                    //     fprintf(stderr,"[*] Error in initializing bloom filter.\n");
                    //     exit(-1);
                    // }
                        // printf("j:%d \n",j);
                        // printf("k:%d \n",k);
                        // printf("cp start2\n");
                        // printf("get data[%d]\n",Abovenumberofnodes);
                        bft->data[Abovenumberofnodes] = copy(bft->data[j]);
                        bft->data[Abovenumberofnodes]->leftparentnode = j;
                        #ifdef LOGGING
                            printf("bft->data[%d] is copyed from mother node : bft->data[%d] \n",Abovenumberofnodes-k,j);
                        #endif 
                        // printfdataarray(bft->data[Abovenumberofnodes]);
                        // printf("bft->data[%d]->leftparentnode = %d\n",Abovenumberofnodes,j);
                        // printf("bft->data[%d]->rightparentnode = %d\n",Abovenumberofnodes,bft->data[Abovenumberofnodes]->rightparentnode);

                        state = 1;
                        k++;
                    }
                    else{
                        // printf("j:%d \n",j);
                        // printf("k:%d \n",k);
                        // printf("union start2\n");
                        // printf("get data[%d]\n",Abovenumberofnodes);
                        bft->data[Abovenumberofnodes] = bf_union(bft->data[j - 1], bft->data[j]);
                        bft->data[Abovenumberofnodes]->leftparentnode = j-1;
                        bft->data[Abovenumberofnodes]->rightparentnode = j;
                        #ifdef LOGGING
                            printf("bft->data[%d] is generated by leftparentnode:bft->data[%d] and rightparentnode bft->data[%d] \n",Abovenumberofnodes-k,j-1,j);
                        #endif 
                        // printf("bft->data[%d]->leftparentnode = %d\n",Abovenumberofnodes,j-1);
                        // printf("bft->data[%d]->rightparentnode = %d\n",Abovenumberofnodes,j);
                        // printfdataarray(bft->data[Abovenumberofnodes]);
                        // printf("union finish\n");
                        k++;
                        state = 0;
                    }
                }             
            }
            
            Currentnumberofnodes = (Currentnumberofnodes+1)/2;
            // printf("Currentnumberofnodes:%d \n",Currentnumberofnodes);
            Abovenumberofnodes = countTheNumberOfNodesAboveTheseNodes(Currentnumberofnodes);
            // printf("Abovenumberofnodes:%d \n",Abovenumberofnodes);
            totalnodes = Currentnumberofnodes + Abovenumberofnodes;
            // printf("union finish finish\n");
        }   
        // printf("union finish finish finish\n");
        bft->built = true;
    }
}


DOUBLENODEBLOOMFILTER *copy(DOUBLENODEBLOOMFILTER *bf1){

    DOUBLENODEBLOOMFILTER *ret = init_empty_BF(bf1->size);
    for(unsigned int i = 0; i < ret->size * 8; i++)
        // if(ChosenCellLocationIsChosen(bf1,i/8,i) || ChosenCellLocationIsChosen(bf2,i/8,i))
        if(ChosenCellLocationIsChosen(bf1,i/8,i))
            setChosenCellTo1(ret,i/8,i);
    // printf("finish cpy\n");
    return ret;
}

DOUBLENODEBLOOMFILTER *bf_union(DOUBLENODEBLOOMFILTER *bf1,DOUBLENODEBLOOMFILTER *bf2){
    if(bf1->size != bf2->size){
        fprintf(stderr, "Cannot perform union on bloom filters of different sizes.");
        exit(1);
    }
    // printf("union \n");
    DOUBLENODEBLOOMFILTER *ret = init_empty_BF(bf1->size);
    // printf("ret->size:%d\n",ret->size);
    for(unsigned int i = 0; i < ret->size * 8; i++){

        if(ChosenCellLocationIsChosen(bf1,i/8,i) || ChosenCellLocationIsChosen(bf2,i/8,i)){
            // printf("bf1 %d ChosenCellLocationIsChosen %d\n",i/8,ChosenCellLocationIsChosen(bf1,i/8,i));
        
            setChosenCellTo1(ret,i/8,i);
        }
    }

    return ret;
}