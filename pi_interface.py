#!/usr/bin/env python

import paramiko
import picamera
import os
import subprocess
from pygame import mixer
from time import sleep

def copyphoto(ssh):
	ftp = ssh.open_sftp() 
	ftp.put('/slotbot/images/pi_frame.jpg', '/slotbot/images/pi_frame.jpg') 
	ftp.close()

def runapp(ssh):
	stdin, stdout, stderr = ssh.exec_command("/home/slotbot_code/build/src/./slotbot")
	output = stdout.readlines()
	return output[0]

def readanswer(ssh):
	stdin, stdout, stderr = ssh.exec_command("/slotbot/./read_answer")
	return stdout.readlines()[0]

# set up audio output
mixer.init()
ansbeep   = mixer.Sound('/slotbot/audio/ansbeep.wav')
errbeep   = mixer.Sound('/slotbot/audio/error.wav')
bellbeep  = mixer.Sound('/slotbot/audio/bell.wav')
noansbeep = mixer.Sound('/slotbot/audio/no_answer.wav')
ansbeep.play()

# set the mothership settings
mothership_ip 		= "..." 
mothership_username = "..."
mothership_password	= "..."

# create PiCamera class instance and change settings
camera = picamera.PiCamera()
camera.sharpness = 0
camera.resolution = (1000,800)
camera.contrast = 0
camera.brightness = 50
camera.saturation = 0
camera.ISO = 0
camera.video_stabilization = False
camera.exposure_compensation = 0
camera.exposure_mode = 'auto'
camera.meter_mode = 'average'
camera.awb_mode = 'auto'
camera.image_effect = 'negative'
camera.color_effects = None
camera.rotation = 0
camera.hflip = False
camera.vflip = False
camera.crop = (0.0, 0.0, 1.0, 1.0)

while True:
	#try:
	# connect with the mothership
	print "\nConnecting with mothership..."
	ssh = paramiko.SSHClient()
	ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())
	ssh.connect(mothership_ip, username='...', password='...')
	if ssh == False:
		raise ValueError('Connection lost')

	print "Successfully connected with mothership!\n\nPerforming operations..."
	bellbeep.play()

	# perform operations in infinite loop
	while True:
		camera.capture('/slotbot/images/pi_frame.jpg')
		copyphoto(ssh)
		answer = runapp(ssh)
		command = "espeak -ven-us+f4 -a 200 -s 280 --stdout '" + answer + "' | aplay -Dplug:bluetooth &"
		subprocess.Popen([command], shell=True)


