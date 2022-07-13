#(c) Copywrite 2022# Jonthan Gunderson

# set async_mode to 'threading', 'eventlet', 'gevent' or 'gevent_uwsgi' to
# force a mode else, the best mode is selected automatically from what's
# installed






#imports for communication between unreal and gymwrapper

import asyncio
#from curses import termname
from multiprocessing.connection import Listener
import time
from turtle import shape
from flask import Flask
import logging
import logging.handlers
from grpc import server
from joblib import Memory
import socketio

import eventlet
from sympy import true
from tensorboard import program
import tensorforce 
import tornado


import json
import random
import numpy as np
import operator
import sys
import os

#from gym import spaces
from random import randint

from threading import Timer


import ast
import math
#import gym

import tensorflow as tf
import tensorforce
from tensorforce.environments import Environment
#from tensorforce.environments import EnvironmentWrapper
from tensorforce.environments import OpenAIGym
from tensorforce.agents import Agent
from tensorforce.execution import Runner
from tensorforce.agents import TrustRegionPolicyOptimization


#tf.compat.v1.disable_eager_execution()
global evn
global sio
global sid

thread = None
observations = "NaN"
UEreward = "0"
UEdone = False
maxactions = 0
obsflag = 0
inf = math.inf
actionspace = "Nan"
observationspace = "Nan"
results = os.getenv('APPDATA')   
lessionTerminated = False
executionComplete = False
isUnrealexecompltete= False
actions = None
internals = None
LessonsLeftCounter = 0

agent =None
NewStateToPassIntoUnreal  =None


import tornado.ioloop
from tornado.iostream import IOStream
import errno
import functools
import socket



   

temp = 0




class CustomEnvironment(Environment): 
    

    def __init__(self):
        super().__init__()

        global actionspace
        global observationspace
        global executionComplete
        global temp
        
        global NewStateToPassIntoUnreal
        
    def states(self):
        print("Statis called")
        #return dict(type= 'float', shape=(8,))
        return dict(type= 'float', shape=(8,),min_value = -1, max_value = 1)

    def actions(self):
        print("Actions called")
        #return dict(type='float', shape=(10,))
        return dict(type='float', shape=(10,), min_value = -1, max_value = 1)
    
    
    
    
    def close(self):
        super().close()

    def reset(self):
        state = np.random.random(size=(8,))
        return state


    def execute(self, actions):
       #Now we need to wait for a response from Unreal engine 

        terminal = False
        global temp
        
        temp = temp +1
        print(temp)
        if temp > 20:
            terminal = True
            temp = 0

        ############ORGINAL REF CODE
        next_state = np.random.random(size=(8,))
          # Always False if no "natural" terminal state
        reward = np.random.random()

        
        print("Execute called")
        return next_state, terminal, reward




    



def receive():
    
    
    global sio
    
    global NewStateToPassIntoUnreal
    global isUnrealexecompltete
    global agent
    global env
    global UEdone
    global reward
    global maxactions
    global conactionspace
    global disactionspace
    global actionspace
    global observationspace
    global LessonsLeftCounter
    global obsdata
    obsdata = None


    algselected = True
    if algselected == True:
    #{
        networkparm='auto'    

        #env = CustomEnvironment()

        print(np.random.random(size=(42,)))
        print(dict(type='float', shape=(42,)))
        
        #agent = TrustRegionPolicyOptimization(env.states(), env.actions(), max_episode_timesteps = 7777, batch_size=10, network=networkparm, use_beta_distribution=False, memory='minimum', update_frequency=1.0, learning_rate=0.01, linesearch_iterations=10, subsampling_fraction=1.0, discount=0.99, reward_processing=None, return_processing=None, advantage_processing=None, predict_terminal_values=False, baseline=None, baseline_optimizer=None, state_preprocessing='linear_normalization', exploration=0.0, variable_noise=0.0, l2_regularization=0.0, entropy_regularization=0.0, parallel_interactions=1, config=None, saver=None, summarizer=None, tracking=None, recorder=None)
        
        LessonsLeftCounter = 100 #arbitrary nuimber as of now
        #agent: Agent = Agent.create(agent='trpo', environment= env, max_episode_timesteps = 7777, batch_size=10, network=networkparm, use_beta_distribution=False, memory='minimum', update_frequency=1.0, learning_rate=0.01, linesearch_iterations=10, subsampling_fraction=1.0, discount=0.99,  return_processing=None, advantage_processing=None, predict_terminal_values=False, baseline=None, baseline_optimizer=None, state_preprocessing='linear_normalization', exploration=0.0, variable_noise=0.0, l2_regularization=0.0, entropy_regularization=0.0, parallel_interactions=1, config=None, saver=None, summarizer=None, tracking=None, recorder=None)
        #agent: Agent = Agent.create(agent='trpo',states = env.states(), actions = env.actions(), max_episode_timesteps = 7777, batch_size=10)

        #runner = Runner(agent, environment=env)
        #runner.run(num_episodes=100)
        
        
        #internals = agent.initial_internals()
       # temptest = None
        #this is the number of training lessions
        '''
        counter = 0
        for _ in range(100):
            states = env.reset()
            reward = 0
            terminal = False
            
            while not terminal:
                counter = counter + 1
                print(counter)
                
                
                if counter > 20:
                    agent.reset()
                    terminal =True
                agent.timestep_completed[0]= False
                
                actions = agent.act(states=states, independent= True, deterministic=False, internals = internals)
                print("Action complete")
                #SEND ACTION ASKED BY A.I. TO UNREAL ENGINE

                
                #WAIT FOR UNREAL TO RESPOND WITH THE RESULTS
                states, terminal, reward = env.execute(actions=actions)
                temptest = agent.observe(terminal=True, reward=reward)
                print("Obersvation complete")
                #states, terminal, reward = env.execute(actions=actions)
            
            
            
            
            agent.update()
        print("Finished")
        
        '''
            

        #} 
        
    
        
        

def fucntiontheloop(agent, env, jsonInput, states, internals):
    global executionComplete

    global actions

    
    #complete a session of a training lession until the actor messes up then restart anther training lesson
    agent.timestep_completed[0]= False
    
    actions = agent.act(states=states, independent= True, deterministic=False, internals = internals)
    print("Action complete")


    #Uncomment later 
    #straction = actions
    straction = "Sent Action From a.i."
    
    #Send The actions to unreal for it too complete
    sio.emit('reactionfromgaia',straction)
    pass



def sendobs(sid, obsdata):
    global observations
    global UEreward
    global UEdone
    global agent
    global env
    global lessionTerminated
    global LessonsLeftCounter
    global actions
    global internals

    terminalButNotUsedOnlyFIllere = False # This is a filler, We will be using Unreal Engine to deciede the lession termination
    
    print("Mindmaker recieved observations and reward from UE, passing back an action")
    jsonInput = json.loads(obsdata)
    observations = jsonInput['observations']     
    UEreward = jsonInput['reward']
    reward = 1
    UEsessioonDone = jsonInput['done']
    lessionTerminated = UEsessioonDone

    
    
    #Check If The actor in unreal did a booboo and reset the lession
    if lessionTerminated == True:  
        agent.update()
        states = env.reset()
        internals = agent.initial_internals()

        
        print("Hold up! wait a minute!")
        print("Lession: " + str(LessonsLeftCounter) + " Finished")
        counter= 3
        while counter>1:
            counter = counter - 1
            print(counter)
            time.sleep(1)
        
  
        LessonsLeftCounter = LessonsLeftCounter - 1
        
        pass
    
    agent.timestep_completed[0]= False
    
    print("Started Observation")
    agent.observe(terminal= lessionTerminated, reward=reward, parallel=0)
    print("Obersvation complete")
    
    states, terminalButNotUsedOnlyFIllere, reward = env.execute(actions=actions)
                

    #Continute the loop
    fucntiontheloop(agent, env, jsonInput, states, internals)
    



        
#This sets up the server connection, with UE acting as the client in a socketIO relationship, will default to eventlet    
if __name__ == '__main__':


    # deploy with eventlet


    print("GAIa statrted... waiting for Unreal Engine to connect")
    time.sleep(1)
    # deploy with eventlet
    receive()

    




