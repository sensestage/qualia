/*
 * DataNetworkEnvironment.cpp
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

#include "DataNetworkEnvironment.h"

#include <unistd.h>
#include <stdio.h>

// the number after currentObservation is the number of inputs, data we listen to
DataNetworkEnvironment::DataNetworkEnvironment(const char *hostip, const char *myport, const char *myName, int auid, int liid, int setid, int outid) : currentObservation(3) {
  
  audioid = auid;
  lightid = liid;
  settingsid = setid;
  outputid = outid;
  
  // create a data network:
    dn = new DataNetwork();
  // create an osc client interface for it:
    dn->createOSC( hostip, myport, myName );
  // register with the host:
    dn->registerMe();

    // subscribe to nodes of interest:
    dn->subscribeNode( audioid, true );
    dn->subscribeNode( lightid, true );
    dn->subscribeNode( settingsid, true );

    // create a node:
    dn->createNode( outputid, myName, 2, 0, true );

    
    
    settingsNode = dn->getNode( settingsid );
    audioNode = dn->getNode( audioid );
    lightNode = dn->getNode( lightid );
    outNode = dn->getNode( outputid );
    
}

// here the environment is initialised, so registering with the data network,
// subscribing to the DataNodes, creating our DataNode
void DataNetworkEnvironment::init() {
  printf("Initializing\n");
}

Observation* DataNetworkEnvironment::start() {
  printf("Starting env\n");

  return &currentObservation;
}

Observation* DataNetworkEnvironment::step(const Action* action) {
  printf("Stepping env\n");
  printf("--> sending %d, %d\n", action->actions[0], action->actions[1]);

  float outData[2];
	// set data to the node:
  for ( int i=0; i<2; i++ ){
    outData[i] = (float) action->actions[i];
  }
  outNode->setData( 2, outData );
	// send the data to the network:
  outNode->send( true );

  if ( settingsNode == NULL ){
    settingsNode = dn->getNode( settingsid );
  }
  
  float delay = 1;
  if ( settingsNode != NULL ){
    delay = settingsNode->getSlot(0)->getValue();
  }
  sleep( delay );

  int cnt = 0;
  if ( audioNode == NULL ){
    audioNode = dn->getNode( audioid );
  }
  if ( audioNode != NULL ){
    float * nodeDataAu = audioNode->getData();
    for ( int i=0; i<audioNode->size(); i++ ){
      currentObservation[cnt] = nodeDataAu[i];
      cnt++;
    }
  }
  if ( lightNode == NULL ){
    lightNode = dn->getNode( lightid );
  }
  if ( lightNode != NULL ){
    float * nodeDataLi = lightNode->getData();
    for ( int i=0; i<lightNode->size(); i++ ){
      currentObservation[cnt] = nodeDataLi[i];
      cnt++;
    }
  }
  float * nodeDataLi = lightNode->getData();

  printf("--> receiving %f, %f, %f\n", currentObservation[0], currentObservation[1], currentObservation[2]);
  //usleep(100);
  return &currentObservation;
}
