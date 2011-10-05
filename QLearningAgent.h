/*
 * QLearningAgent.h
 *
 * An agent that behaves according to an action-value, TD-lambda reinforcement
 * learning algorithm. The model allows for both on-policy (SARSA) and off-policy
 * (Q learning) learning.
 *
 * This file is part of Qualia https://github.com/sofian/qualia
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
#ifndef QLEARNINGAGENT_H_
#define QLEARNINGAGENT_H_

#include "Agent.h"
#include "NeuralNetwork.h"
#include "RLObservation.h"
#include "Policy.h"

class QLearningAgent : public Agent {

public:
//  real *_softmax; // softmax probabilities (internal use)
  NeuralNetwork* function;

  // TODO: if we ever make subclasses of Action we will need to change this...
  Action currentAction;
  Action bufferAction;
  RLObservation lastObservation;

  // Parameters.
  real *e; // elligibility traces

  unsigned int observationDim;
  unsigned long nConflatedActions;
  float lambdaTimesGamma; // lambda is always used like that...
  float gamma;

  Policy* policy;

  bool qLearning; // use Q-Learning (off-policy) instead of Sarsa

  // Internal use.
  real *nnInput; // a bit inefficient

  QLearningAgent(NeuralNetwork* func,
                 unsigned int observationDim, unsigned int actionDim, const unsigned int* nActions,
                 float lambda, float gamma, Policy* policy, bool qLearning = false);
  virtual ~QLearningAgent();

  virtual void init();
  virtual Action* start(const Observation* observation);
  virtual Action* step(const Observation* observation);
  virtual void end(const Observation* observation);
//  virtual void cleanup();

  real Q(const Observation* observation, const Action* action);

  // dst is optional (won't be recorded if set to NULL)
  void getMaxAction(Action* dst, const Observation* observation, real *maxQ = 0);

};

#endif /* QLEARNINGAGENT_H_ */
