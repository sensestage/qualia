//#include "cpp_include.h"

#include "Qualia.h"
// #include "DummyAgent.h"
#include "QLearningAgent.h"
#include "QLearningEGreedyPolicy.h"
#include "NeuralNetwork.h"
#include "RLQualia.h"

#include "PolytopeLightEnvironment.h"

//#define STATIC_ALLOCATOR_SIZE 10000
//#include "StaticAllocator.h"

/// this is the only file we need to include to interact with the datanetwork
#include "datanetwork.h"

// #define N_HIDDEN 3
#define DIM_OBSERVATIONS 2
#define DIM_ACTIONS 2
const unsigned int n_actions[] = { 4, 200 }; // { off, on, flash, flicker ; duration }

#include <stdio.h>

/// this is the namespace used by the datanetwork classes
using namespace SWDataNetwork;

//unsigned char buffer[STATIC_ALLOCATOR_SIZE];
//StaticAllocator myAlloc(buffer, STATIC_ALLOCATOR_SIZE);
int main(int argc, char *argv[]) {
  
    if ( argc < 7 ){
    printf( "Start this DataNetwork Qualia client with 7 arguments:\n");
    printf( "host ip (e.g. 127.0.0.1), the IP address of the host running the DataNetwork server\n");
    printf( "port (e.g. 7000), the port this client will use for OSC messages\n");
    printf( "name (e.g. example_client), the name by which this client will be identified in the DataNetwork\n");
    printf( "node for light input data, node for settings, node for output\n");
    printf( "lambda, gamma, epsilon, learning rate, hidden layers, reward node\n");
    printf( "For example:\n");
    printf( "%s 127.0.0.1 7000 QualiaClient 37 4001 5001 0.9 0.1 0.5 0.1 3 3001\n", argv[0]); 
    return EXIT_SUCCESS;
  }

  float lambda = atof( argv[7] );
  float gamma = atof( argv[8] );
  float epsilon = atof( argv[9] );
  float learning = atof( argv[10] );
  int n_hidden = atoi( argv[11] );

  float settingsid = atoi( argv[5] );
  DataNode * settingsNode = NULL;
  
  // initialise datanetwork:
  // create a data network:
  DataNetwork * dn = new DataNetwork();
  // create an osc client interface for it:
  dn->createOSC( argv[1], argv[2], argv[3] );
  // register with the host:
  dn->registerMe();
  dn->subscribeNode( settingsid, true );

  //Alloc::init(&myAlloc);
//   DummyAgent agent;
  QLearningEGreedyPolicy egreedy( epsilon );
  NeuralNetwork net(DIM_OBSERVATIONS + DIM_ACTIONS, n_hidden, 1, learning);
  QLearningAgent agent(&net, DIM_OBSERVATIONS, DIM_ACTIONS, n_actions, lambda, gamma, &egreedy, false);
  // lambda = 1.0 => no history
  // gamma
  PolytopeLightEnvironment * env = new PolytopeLightEnvironment( dn, argv[3], atoi(argv[4]), atoi(argv[12]), atoi(argv[6]) );
  Qualia qualia(&agent, env);
//  RLQualia qualia(&agent, &env);

  qualia.init();
  qualia.start();

  for (;;) {
    if ( settingsNode == NULL ){
      settingsNode = dn->getNode( settingsid );
    }
    if ( settingsNode != NULL ){
      env->set_delay( settingsNode->getSlot(0)->getValue() );
      agent.lambda = settingsNode->getSlot(1)->getValue();
      agent.gamma = settingsNode->getSlot(2)->getValue();
      egreedy.epsilon = settingsNode->getSlot(3)->getValue();
      net.learningRate = settingsNode->getSlot(4)->getValue();
    }
//  for (int i=0; i<10; i++) {
    qualia.step();

// #if is_computer()
//     printf("Current agent action: %d\n", agent.currentAction.conflated());
//     printf("Current environment observation: %f %f\n", (double)env->currentObservation.observations[0], (double)env->currentObservation.observations[1]);
// #endif
  }

//  if (myAlloc.nLeaks)
//    printf("WARNING: Static Allocator has leaks: %d\n", myAlloc.nLeaks);

  return 0;
}

