/*
 * PolytopeLightEnvironment.cpp
 *
 * (c) 2011 Marije Baalman -- nescivi(@)gmail(.)com
 * (c) 2011 Sofian Audry -- info(@)sofianaudry(.)com
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "PolytopeLightEnvironment.h"

#include <unistd.h>
#include <stdio.h>

// the number after currentObservation is the number of inputs, data we listen to
PolytopeLightEnvironment::PolytopeLightEnvironment( DataNetwork * d, const char* myName, int liid, int rewid, int outid) : currentObservation(2) {
  
  lightid = liid;
  outputid = outid;
  dn = d;
  delay = 1.;
  rewardid = rewid;  
  
    // subscribe to nodes of interest:
    dn->subscribeNode( lightid, true );
    dn->subscribeNode( rewardid, true );

    // create a node:
    dn->createNode( outputid, myName, 2, 0, true );

    lightNode = dn->getNode( lightid );

    rewardNode = dn->getNode( rewardid );

    outNode = dn->getNode( outputid );    
}

void PolytopeLightEnvironment::set_delay( float d ){
  delay = d;
}
  
// here the environment is initialised, so registering with the data network,
// subscribing to the DataNodes, creating our DataNode
void PolytopeLightEnvironment::init() {
//   printf("Initializing\n");
}

Observation* PolytopeLightEnvironment::start() {
//   printf("Starting env\n");

  return &currentObservation;
}

Observation* PolytopeLightEnvironment::step(const Action* action) {
//   printf("Stepping env\n");
//   printf("--> sending %d, %d\n", action->actions[0], action->actions[1] );

  float outData[2];
	// set data to the node:
  for ( int i=0; i<2; i++ ){
    outData[i] = (float) action->actions[i];
  }
  outNode->setData( 2, outData );
	// send the data to the network:
  outNode->send( true );

  sleep( delay );

  if ( lightNode == NULL ){
    lightNode = dn->getNode( lightid );
  }
  if ( lightNode != NULL ){
    float * nodeDataLi = lightNode->getData();
    for ( int i=0; i<lightNode->size(); i++ ){
      currentObservation[i] = nodeDataLi[i];
    }
  }

  float reward = 0;
  if ( rewardNode == NULL ){
    rewardNode = dn->getNode( rewardid );
  }
  if ( rewardNode != NULL ){
    float * rewardData = rewardNode->getData();
    for ( int i=0; i<rewardNode->size(); i++ ){
//       currentObservation[i] = nodeDataAu[i];
      reward += rewardData[i];
    }
  }

    // Darkness reward.
  currentObservation.reward = reward;

//   printf("--> receiving %f, %f\n", currentObservation[0], currentObservation[1]);
  //usleep(100);
  return &currentObservation;
}
