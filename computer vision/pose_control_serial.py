import cv2
import mediapipe as mp
import numpy as np
import time
import serial

arduino = serial.Serial(port='/dev/cu.usbmodem144101', baudrate=144101, timeout=.1)

mp_drawing = mp.solutions.drawing_utils
mp_drawing_styles = mp.solutions.drawing_styles
mp_pose = mp.solutions.pose


def write_read(x):
    arduino.write(bytes(x, 'utf-8'))
    
    #time.sleep(0.05)
    data = arduino.readline()
    return data


def calculate_angle(A_x, A_y, B_x, B_y, C_x, C_y):
    a = np.array([A_x, A_y])
    b = np.array([B_x, B_y])
    c = np.array([C_x, C_y])

    ba = a - b
    bc = c - b

    cosine_angle = np.dot(ba, bc) / (np.linalg.norm(ba) * np.linalg.norm(bc))
    angle = np.arccos(cosine_angle)
    angle = int(np.degrees(angle))

    return angle

angle_list = []
angle_output = 0

back = None
num = str(0)+ "\n"
pre_num = 0
back =  write_read(num)
time.sleep(0.5)

cap = cv2.VideoCapture(0)
with mp_pose.Pose(
    min_detection_confidence=0.5,
    min_tracking_confidence=0.5) as pose:
  while cap.isOpened():
    success, image = cap.read()
    if not success:
      print("Ignoring empty camera frame.")
      break

    image.flags.writeable = False
    image = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
    image = cv2.flip(image, 1)
    image_width, image_height = image.shape[1], image.shape[0]
    results = pose.process(image)

    # Draw the pose annotation on the image.
    # image.flags.writeable = True
    image = cv2.cvtColor(image, cv2.COLOR_RGB2BGR)
    # mp_drawing.draw_landmarks(
    #     image,
    #     results.pose_landmarks,
    #     mp_pose.POSE_CONNECTIONS,
    #     landmark_drawing_spec=mp_drawing_styles.get_default_pose_landmarks_style())
    
    # calculate angles of the right arm
    landmark_list = []
    if results.pose_landmarks:
      for id, landmark in enumerate(results.pose_landmarks.landmark):
          x, y = int(landmark.x * image_width), int(landmark.y * image_height)
          vis = landmark.visibility
          landmark_list.append([id, x, y, vis])
      Rshoulder_x = landmark_list[12][1]
      Rshoulder_y = landmark_list[12][2]
      Rshoulder_vis = landmark_list[12][3]
      Relbow_x = landmark_list[14][1]
      Relbow_y = landmark_list[14][2]
      Relbow_vis = landmark_list[14][3]
      Rwrist_x = landmark_list[16][1]
      Rwrist_y = landmark_list[16][2]
      Rwrist_vis = landmark_list[16][3]

      cv2.circle(image, (Rshoulder_x, Rshoulder_y), 10, (0, 0, 255), cv2.FILLED)
      cv2.circle(image, (Relbow_x, Relbow_y), 10, (0, 0, 255), cv2.FILLED)
      cv2.circle(image, (Rwrist_x, Rwrist_y), 10, (0, 0, 255), cv2.FILLED)

      if (Rshoulder_vis > 0.80 and Relbow_vis > 0.80 and Rwrist_vis > 0.80):
        angle = calculate_angle(Rshoulder_x, Rshoulder_y, Relbow_x, Relbow_y, Rwrist_x, Rwrist_y)
    #   print("Rshoulder:", Rshoulder_x, Rshoulder_y)
    #   print("Relbow:", Relbow_x, Relbow_y)
    #   print("Rwrist:", Rwrist_x, Rwrist_y)
    #   print("visibility:", Rshoulder_vis, Relbow_vis, Rwrist_vis)
        angle_list.append(angle)

      if(len(angle_list) == 5):
          angle_output = np.average(angle_list)
          angle_list = []
          # arduino.write(str(angle_output).encode())
          # time.sleep(1)
          #print(arduino.readline().decode('ascii'))

          # this part works
          if back is not None:
            back = None
            num = str(angle_output) + "\n"
            back = write_read(num)

          # try to smooth the movement
          # if back is not None:
          #   back = None
          #   angle_diff = angle_output - pre_num
          #   if (angle_diff > 50):
              

          print("angle:", angle_output)

      # if i == 20:
      #   arduino.write(str(angle_output).encode())
      #   print(arduino.readline().decode('ascii'))
      #   i = 0

      
      

    # Flip the image horizontally for a selfie-view display.
      cv2.putText(image, "angle:{}".format(angle_output), (10, 120), cv2.FONT_HERSHEY_PLAIN, 3,
      (255, 0, 255), 3)
    cv2.imshow('MediaPipe Pose', image)

    if cv2.waitKey(5) & 0xFF == 27:
      break
cap.release()