from os import terminal_size
import cv2
import mediapipe as mp
import numpy as np
import time
import serial
import threading


data_lock = threading.Lock()
data_holder = None
stop_threads = False

arduino = serial.Serial(port='/dev/cu.usbmodem144101', baudrate=115200, timeout=None)

mp_drawing = mp.solutions.drawing_utils
mp_drawing_styles = mp.solutions.drawing_styles
mp_pose = mp.solutions.pose

def transmit_data_func():
  print("thread started")
  while True:
    if stop_threads:
      print ('* Received keyboard interrupt, quitting threads.')
      break
    _timer_start = time.time()
    data_lock.acquire()
    if data_holder is not None:
      data = str(data_holder).encode()
      _timer_takein = time.time()
      data_lock.release()
    else:
      data_lock.release()
      continue
    _timer_acquire = time.time()

    arduino.write(data)  
    time.sleep(1)  
    _timer_write = time.time()
    # status = arduino.readline().decode('ascii')
    # print(status)
    
    _timer_read = time.time()
    print ("timing: takein (%.5f)acc (%.5f) write (%.5f) read (%.5f)" % (
      (_timer_takein - _timer_start),
      (_timer_acquire - _timer_takein),
      (_timer_write - _timer_acquire),
      (_timer_read - _timer_write),
    ))

transmit_thread = threading.Thread(
  target=transmit_data_func, name= 'transmit_thread'
)
transmit_thread.start()


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

# return_item = "success"
# t1 = threading.Thread(target=transmit_data, name= 't1')
angle_list = []
angle_output = None
cap = cv2.VideoCapture(0)

try:
  with mp_pose.Pose(
      min_detection_confidence=0.5,
      min_tracking_confidence=0.5) as pose:
    while cap.isOpened():
      main_timer_start = time.time()
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
          angle_list.append(angle)
          # print("trustable data")
          # print("Rshoulder:", Rshoulder_x, Rshoulder_y)
          # print("Relbow:", Relbow_x, Relbow_y)
          # print("Rwrist:", Rwrist_x, Rwrist_y)
          # print("visibility:", Rshoulder_vis, Relbow_vis, Rwrist_vis)
        
        #main_timer_start = time.time()
        if(len(angle_list) == 5):
            angle_output = np.average(angle_list)
            angle_list = []
            #main_timer_angle = time.time()
            data_lock.acquire()
            data_holder = angle_output
            data_lock.release()
            print("angle:", angle_output)
            main_timer_data = time.time()
            print ("timing: data(%.5f)" % (
              (main_timer_data - main_timer_start),
            ))

      # Flip the image horizontally for a selfie-view display.
        cv2.putText(image, "angle:{}".format(angle_output), (10, 120), cv2.FONT_HERSHEY_PLAIN, 3,
        (255, 0, 255), 3)
      cv2.imshow('MediaPipe Pose', image)

      if cv2.waitKey(5) & 0xFF == 27:
        break

except (KeyboardInterrupt, SystemExit):
  print ('\n* Received keyboard interrupt, quitting main.')
  stop_threads = True
  cap.release()
  transmit_thread.join()