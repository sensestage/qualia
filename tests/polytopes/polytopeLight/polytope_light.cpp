//#include "cpp_include.h"

#include "Qualia.h"
#include "DummyAgent.h"
#include "QLearningAgent.h"
#include "QLearningEGreedyPolicy.h"
#include "PolytopeLightEnvironment.h"
#include "NeuralNetwork.h"
//#include "RLQualia.h"

//#define STATIC_ALLOCATOR_SIZE 10000
//#include "StaticAllocator.h"

#define N_HIDDEN 3
#define DIM_OBSERVATIONS 2
#define DIM_ACTIONS 2
const unsigned int n_actions[] = { 4, 200 }; // { off, on, flash, flicker ; duration }

#include <stdio.h>

//unsigned char buffer[STATIC_ALLOCATOR_SIZE];
//StaticAllocator myAlloc(buffer, STATIC_ALLOCATOR_SIZE);
int main(int argc, char *argv[]) {
  
    if ( argc < 7 ){
    printf( "Start this DataNetwork Qualia client with 7 arguments:\n");
    printf( "host ip (e.g. 127.0.0.1), the IP address of the host running the DataNetwork server\n");
    printf( "port (e.g. 7000), the port this client will use for OSC messages\n");
    printf( "name (e.g. example_client), the name by which this client will be identified in the DataNetwork\n");
    printf( "For example:\n");
    printf( "%s 127.0.0.1 7000 QualiaClient 37 4001 5001\n", argv[0]); 
    return EXIT_SUCCESS;
  }

  
  //Alloc::init(&myAlloc);
//   DummyAgent agent;
  QLearningEGreedyPolicy egreedy(0.1f);
  NeuralNetwork net(DIM_OBSERVATIONS + DIM_ACTIONS, N_HIDDEN, 1, 0.1f);
  QLearningAgent agent(&net, DIM_OBSERVATIONS, DIM_ACTIONS, n_actions,
                       1.0f, 0.1f, &egreedy, false); // lambda = 1.0 => no history
  PolytopeLightEnvironment * env = new PolytopeLightEnvironment( argv[1], argv[2], argv[3], atoi(argv[4]), atoi(argv[5]), atoi(argv[6]) ); 
  Qualia qualia(&agent, env);
//  RLQualia qualia(&agent, &env);

  qualia.init();
  qualia.start();

  for (;;) {
//  for (int i=0; i<10; i++) {
    qualia.step();
#if is_computer()
//     printf("Current agent action: %d\n", agent.currentAction.conflated());
//     printf("Current environment observation: %f %f\n", (double)env->currentObservation.observations[0], (double)env->currentObservation.observations[1]);
#endif
  }

//  if (myAlloc.nLeaks)
//    printf("WARNING: Static Allocator has leaks: %d\n", myAlloc.nLeaks);

  return 0;
}
