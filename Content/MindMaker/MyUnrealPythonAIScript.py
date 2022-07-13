#(c) Copywrite 2022# Jonthan Gunderson

# set async_mode to 'threading', 'eventlet', 'gevent' or 'gevent_uwsgi' to
# force a mode else, the best mode is selected automatically from what's
# installed






#imports for communication between unreal and gymwrapper

import asyncio
#from curses import termname
from multiprocessing.connection import Listener
import time
import engineio
from flask import Flask
import logging
import logging.handlers
from grpc import server
from joblib import Memory
#import pandas

from platformdirs import site_data_path
import socketio

import eventlet
from sympy import true
from tensorboard import program 
import tornado


import json
import os
from os.path import exists
import random
import numpy as np
import operator
import sys


#from gym import spaces
from random import randint

from threading import Timer


import ast
import math


import tensorflow as tf

import sys

import tensorforce
from tensorforce.environments import Environment


from tensorforce.agents import Agent
from tensorforce.execution import Runner
from tensorforce.agents import TrustRegionPolicyOptimization



global sid

thread = None
observations = "NaN"
UEreward = "0"
terminate_episode = False


inf = math.inf
INITALactionspace = "Nan"
observationspace = "Nan"
INITALObservationSpace = "Nan"
results = os.getenv('APPDATA')   
executionComplete = False

temp = 0




sio = socketio.Server(logger=True,  engineio_logger=True, async_mode = 'eventlet',ping_timeout = 50000)
app = Flask(__name__)
app.wsgi_app = socketio.WSGIApp(sio, app.wsgi_app)


class CustomEnvironment(Environment): 
    

    def __init__(self):
        super(CustomEnvironment, self).__init__()
        global INITALactionspace
        global observationspace
        global INITALObservationSpace #Only called once at the intitaion of the A.I.
        global executionComplete
        global temp
        global UEreward
        global terminate_episode

        
        
        
    def states(self):
        print("Statis called")
        global INITALObservationSpace
        return INITALObservationSpace
        #return dict(type='float', shape=(42,))

    def actions(self):
        print("Actions called")
        global INITALactionspace
        return INITALactionspace
        #return dict(type='float', shape=30)




    def close(self):
        super().close()

    def reset(self):
        #state = np.random.random(size=(42,))
        global observationspace
        return observationspace
        #return state


    def execute(self, actions):
        global temp
        global executionComplete
        global UEreward
        global terminate_episode
        global observationspace
        terminal = False

        temp = temp + 1
        
        
        straction = str(actions)
        sio.emit('reactionfromgaia',straction)
        
        
        #Buffer to ensure that unreal completed the actions given too it
        executionComplete = False
        # Wait for unreal to response
        while executionComplete == False:         
            sio.sleep(.001)

            
            
            
        #print("Last final reward was: " + str(UEreward))
        if terminate_episode == True:      
            
            print("Updating tensors and resetting environment...") 
            terminal = True
                  

        
        if temp > 0 and temp < 2:
            print("Initial system tensor save and update, this takes a while and im not sure why...")
            terminal = True
            







        #Now we need to wait for a response from Unreal engine 


        ############ORGINAL REF CODE
        next_state = observationspace
        
        
        
        #THis will be filled with the new state of the environment AFTER unreal completed its actions
        #We then feed the new envorment into THe A.I. for it to process its new actions
        #next_state =  dict(shape=(42,))
            # Always False if no "natural" terminal state
        reward = UEreward
        
        return next_state, terminal, reward




    
#}

@sio.event
def disconnect_request(sid):
    sio.disconnect(sid)
    os._exit(1)

@sio.event
def connect(sid, evenlet):
    print ("Connected To Unreal Engine")
    


@sio.event
def disconnect(sid):
    print('Disconnected From Unreal Engine, Exiting G.a.i.A')
    os._exit(1)
    
    
    

 #This is called once at run time
@sio.on('runGaia')
def receive(sid, data):  
    global UEdone
    global conactionspace
    global INITALactionspace
    global observationspace
    global INITALObservationSpace  
    global obsdata
    global terminate_episode
    obsdata = None
    
    
    
    jsonInput = json.loads(data)
    
    
   # actionspace = jsonInput['actionspace']
    #observationspace = jsonInput['observationspace']

    trainepisodes = jsonInput['trainepisodes']
    evalepisodes = jsonInput['evalepisodes']
    loadmodel = jsonInput['loadmodel']
    savemodel = jsonInput['savemodel']
    #modelname = jsonInput['modelname']
    algselected = jsonInput['rlselected']

    

    conactionspace = jsonInput['continousactionspace']
    disactionspace = jsonInput['disactionspace']
    UEdone = jsonInput['done']

  


    #print("save model value:", savemodel)
    #print("load model value:", loadmodel)
    
    
    
    loadmodel = 'true'
    overridemodel = 'false'
    savemodel = 'true'
   # modelname = '6AxisModel_7_charity_smallerRewardSpace'
    modelname = '6AxisModel_7_charity'
    
    path = results + "\\" + modelname
    print("Model of name: " + str(modelname) + " Selected")
    
    
    if loadmodel == 'true' and overridemodel == 'false':
         # Load the trained agent
        print("Loading Model")
        INITALObservationSpace = dict(type='float', shape=(42,))  #This will be retrieved from unreal engine
        INITALactionspace = dict(type='float', shape=30, min_value = -10, max_value = 10) #This will be retrieved from unreal engine
        observationspace = np.random.random(size=(42,)) #This will be retrieved from unreal engine 
        env = CustomEnvironment()
        max_episode_timestep = 500
        
        
        loadAgent = Agent.load(directory = path, filename =modelname)
        runner = runner = Runner(loadAgent, env,max_episode_timesteps = max_episode_timestep)   
        
        print("Running Evaluation Mode...")
        runner.run(num_episodes = 100, evaluation = True)
        
        print("Evaluation Complete!!!")
        exitprogramCountdown = 10
        while exitprogramCountdown > 0:
        #{
            temp = str(exitprogramCountdown)
            print ("Exiting program in: " + temp + " Seconds")
            time.sleep(1)
            exitprogramCountdown = exitprogramCountdown - 1
        #}   
                
        sio.disconnect(sid)
        
        
    elif loadmodel == 'false' or overridemodel == 'true':
        if algselected == 'TRPO':
        #{
            networkparm='auto'    
            '''
            global observationspace
            global INITALObservationSpace  
            global INITALactionspace
            max_episode_timesteps, 
            batch_size, 
            network='auto', 
            use_beta_distribution=False, 
            memory='minimum', 
            update_frequency=1.0, 
            learning_rate=0.01, 
            linesearch_iterations=10, 
            subsampling_fraction=1.0, 
            discount=0.99, 
            reward_processing=None, 
            return_processing=None, 
            advantage_processing=None, 
            predict_terminal_values=False, 
            baseline=None, 
            baseline_optimizer=None, s
            tate_preprocessing='linear_normalization', 
            exploration=0.0, v
            ariable_noise=0.0, 
            l2_regularization=0.0, 
            entropy_regularization=0.0, 
            parallel_interactions=1, 
            config=None, 
            saver=None, 
            summarizer=None, 
            tracking=None, 
            recorder=None
            '''
            #policy= [dict(type='dense', size=1024, activation='tanh'), dict(type='dense', size=2048, activation='tanh')  ,dict(type='dense', size=2048, activation='tanh'), dict(type='dense', size=1024, activation='tanh')]
            

            #Check if file already exists
            if exists(path):
                print("This file already exists!!!")
                print("Do you want to continue training model, create new model , or exit program? (y/n/e)")
                input1 = input()
                if input1 =='y':
                    pass
                elif input1 == 'n':
                    print("Adding subscript to file")
                    newNameCreated = False
                    while newNameCreated == False:
                        modelname_new = modelname + str(random.randint(10, 100))
                        if modelname_new == modelname:
                            newNameCreated = False
                        else:
                            modelname =  modelname_new
                            newNameCreated = True
                    print("Model with name " + str(modelname) + " Created")
                    time.sleep(10)
                elif input1 == 'e':
                    print("Exiting g.A.I.a")
                    sio.disconnect(sid)
                else:
                    print("Unknown response Exiting g.A.I.a for safety")
                    sio.disconnect(sid)
            
            
            
            INITALObservationSpace = dict(type='float', shape=(42,))  #This will be retrieved from unreal engine
            INITALactionspace = dict(type='float', shape=30, min_value = -10, max_value = 10) #This will be retrieved from unreal engine
            observationspace = np.random.random(size=(42,)) #This will be retrieved from unreal engine
            env = CustomEnvironment()
            
            
            LessonsLeftCounter = 5000 #arbitrary number as of how many lessons to teach
            max_episode_timestep = 250 #auto stop episode so that the training does not last forever
            
            if savemodel == 'true' or overridemodel == 'true':
                saver = dict(directory = path, filename = modelname)
                print("Save Model of name "+ str(modelname) + " To Path of " + str(path))
            else:
                saver =None
               
            
            if overridemodel == 'false':

                
                agent: Agent = Agent.create(agent='trpo', environment = env, max_episode_timesteps = 77777, batch_size=10, network=networkparm, use_beta_distribution=False, memory='minimum', update_frequency=1.0, learning_rate=0.001, linesearch_iterations=10, subsampling_fraction=1.0, discount=0.95,  return_processing=None, advantage_processing=None, predict_terminal_values=False, baseline=None, baseline_optimizer=None, state_preprocessing='linear_normalization', exploration=10.0, variable_noise=0.25, l2_regularization=0.0, entropy_regularization=0.0, parallel_interactions=1, config=None, saver=saver, summarizer=None, tracking=None, recorder=None)

                #agent: Agent = Agent.create(agent='ppo',environment = env, max_episode_timesteps = 77777, batch_size=10, network=networkparm, use_beta_distribution=False, memory='minimum', update_frequency=1.0, learning_rate=0.001, multi_step=10, subsampling_fraction=0.33, likelihood_ratio_clipping=0.25, discount=0.95, reward_processing=None, return_processing=None, advantage_processing=None, predict_terminal_values=False, baseline=None, baseline_optimizer=None, state_preprocessing='linear_normalization', exploration=5.0, variable_noise=0.7, l2_regularization=0.0, entropy_regularization=0.0, parallel_interactions=1, config=None, saver=saver, summarizer=None, tracking=None, recorder=None)
            
            elif overridemodel == 'true':
                agent = Agent.load(directory = path, filename =  modelname)
            else:
                print("Failed at loading agent for override")
                exit(1)
            
            
            
            #Start Training
            print("Starting Training Session")
            print("Save Model: " + str(savemodel))
            time.sleep(3)
            

            runner = Runner(agent, env, max_episode_timesteps = max_episode_timestep)    
            runner.run(num_episodes=LessonsLeftCounter)
            
            print("Finished Training Session")
            
            
            if savemodel == 'true' or overridemodel == 'true':
                agent.save(directory = path, filename = modelname)
                print("Saving Model of name "+ str(modelname) + " To Path of " + str(path))
                time.sleep(10)
            
            
            countdownToEvaultion = 10
            while  countdownToEvaultion > 0:
                temp = str( countdownToEvaultion)
                print ("Evaluating Model in: " + temp + " Seconds")
                time.sleep(1)
                countdownToEvaultion = countdownToEvaultion - 1
            
            
            
            print("Running Evaluation Mode...")
            runner.run(num_episodes = 100, evaluation = True)
            







            exitprogramCountdown = 10
            while exitprogramCountdown > 0:
            #{
                temp = str(exitprogramCountdown)
                print ("Exiting program in: " + temp + " Seconds")
                time.sleep(1)
                exitprogramCountdown = exitprogramCountdown - 1
            #}   
                
            sio.disconnect(sid)
    else:
        exit(1)        
        #} 
        
    


#recieves observations and reward from Unreal Engine    
@sio.on('sendGaiaObservationData')
def sendobs(sid, obsdata):
    global observationspace
    global UEreward
    global executionComplete
    global terminate_episode
    


    
    #print("G.a.i.A recieved observations and reward from UE, passing back an action")
    jsonInput = json.loads(obsdata)
    
    observationspace = jsonInput['observations']    # read all observation from Unreal engine in json format
    UEreward = jsonInput['reward']
    terminate_episode = jsonInput['done']
    
    
    
    observationspace  = ast.literal_eval( observationspace)



    
    
    executionComplete = True
    
    #Check If The actor in unreal did a booboo and reset the lession
    
  


    



    


        
   
    

        
#This sets up the server connection, with UE acting as the client in a socketIO relationship, will default to eventlet    
if __name__ == '__main__':


    # deploy with eventlet
    logging.disable(sys.maxsize)

    print("GAIa statrted... waiting for Unreal Engine to connect")
    
    # deploy with eventlet
    import eventlet
    import eventlet.wsgi


    eventlet.wsgi.server(eventlet.listen(('', 3000)), app)


    




