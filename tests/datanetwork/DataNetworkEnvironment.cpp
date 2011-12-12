/*
 * LibMapperEnvironment.cpp
 *
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
DataNetworkEnvironment::DataNetworkEnvironment() : currentObservation(3) {
}

// here the environment is initialised, so registering with the data network,
// subscribing to the DataNodes, creating our DataNode
void DataNetworkEnvironment::init() {
  printf("Initializing\n");
  // create a data network:
    dn = new DataNetwork();
  // create an osc client interface for it:
    dn->createOSC( "127.0.0.1", "7000", "qualia" );
  // register with the host:
    dn->registerMe();

    // subscribe to nodes of interest:
    dn->subscribeNode( 22, true );
    // subscribe to nodes of interest:
    dn->subscribeNode( 37, true );

	// create a node:
    dn->createNode( 5001, "qualia1", 2, 0, true );

    delayNode = dn->getNode( 6001 );
    node21 = dn->getNode( 22 );
    node37 = dn->getNode( 37 );
    outNode = dn->getNode( 5001 );
}

Observation* DataNetworkEnvironment::start() {
  printf("Starting env\n");

  return &currentObservation;
}

Observation* DataNetworkEnvironment::step(const Action* action) {
  printf("Stepping env\n");
  printf("--> sending %d\n", action->actions[0]);

  float outData[2];
	// set data to the node:
  for ( int i=0; i<2; i++ ){
	  outData[i] = (float) action->actions[i];
  }
  outNode->setData( 2, outData );
	// send the data to the network:
  outNode->send( true );

  sleep( 1 );

  float * nodeData21 = node21->getData();
  float * nodeData37 = node37->getData();
  int cnt = 0;
  for ( int i=0; i<node21->size(); i++ ){
	  currentObservation[cnt] = nodeData21[i];
	  cnt++;
  }
  for ( int i=0; i<node37->size(); i++ ){
	  currentObservation[cnt] = nodeData37[i];
	  cnt++;
  }

  printf("--> receiving %f\n", currentObservation[0]);
  //usleep(100);
  return &currentObservation;
}
