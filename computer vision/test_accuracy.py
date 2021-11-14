import cv2
import mediapipe as mp
import time
import numpy as np


 
 
class handDetector():
    def __init__(self, mode=False, maxHands=2, detectionCon=0.5, trackCon=0.5):
        self.mode = mode
        self.maxHands = maxHands
        self.detectionCon = detectionCon
        self.trackCon = trackCon
 
        self.mpHands = mp.solutions.hands
        self.hands = self.mpHands.Hands(self.mode, self.maxHands,
                                        self.detectionCon, self.trackCon)
        self.mpDraw = mp.solutions.drawing_utils
 
    def findHands(self, img, draw=True):
        imgRGB = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
        self.results = self.hands.process(imgRGB)
        # print(results.multi_hand_landmarks)
 
        if self.results.multi_hand_landmarks:
            for handLms in self.results.multi_hand_landmarks:
                if draw:
                    self.mpDraw.draw_landmarks(img, handLms,
                                               self.mpHands.HAND_CONNECTIONS)
        return img
 
    def findPosition(self, img, handNo=0, draw=True):
 
        lmList = []
        z = []
        average_depth = 0
        if self.results.multi_hand_landmarks:
            for myHand in self.results.multi_hand_landmarks:


                for id, lm in enumerate(myHand.landmark):
                    # print(id, lm)
                    h, w, c = img.shape
                    cx, cy, cz = int(lm.x * w), int(lm.y * h), int(lm.z * w)
                    # print(id, cx, cy)
                    lmList.append([id, cx, cy])
                    z.append(cz)
                    # if draw:
                    #     cv2.circle(img, (cx, cy), 15, (255, 0, 255), cv2.FILLED)
                average_depth = abs(np.sum(z) / len(z))
 
        return lmList, average_depth

 
 
def main():
    pTime = 0
    cTime = 0
    cap = cv2.VideoCapture(0)
    detector = handDetector()
    i = 0
    length_relative = -1
    z = -1
    average_depth = -1
    image_list = []
    while True:
        success, img = cap.read()
        
        h, w, c = img.shape
        img = detector.findHands(img)
        print("shape of img:", img.shape)
        lmList, avr_depth = detector.findPosition(img)
        if len(lmList) != 0:
            x_t = lmList[8][1]
            y_t = lmList[8][2]
            x_i = lmList[4][1]
            y_i = lmList[4][2]
            # z = (lmList[8][3] + lmList[4][3]) / 2
            base1_x = lmList[0][1]
            base1_y = lmList[0][2]
            base2_x = lmList[5][1]
            base2_y = lmList[5][2]
            cv2.circle(img,(x_t, y_t), 15, (255,0,255),cv2.FILLED)
            cv2.circle(img,(x_i,y_i), 15, (255,0,255),cv2.FILLED)
            length = np.sqrt((x_t - x_i) * (x_t - x_i) + (y_t - y_i) * (y_t - y_i))
            # length_base = np.sqrt((base1_x - base2_x) * (base1_x - base2_x) + (base1_y - base2_y) * (base1_y - base2_y))
            # length_relative = length / length_base
            length_relative = length / avr_depth
            print(length_relative)

        
        cv2.putText(img, "lengt:{:.3f}".format(length_relative), (10, 120), cv2.FONT_HERSHEY_PLAIN, 3,
                (255, 0, 255), 3)


        cTime = time.time()
        fps = 1 / (cTime - pTime)
        pTime = cTime
 
        cv2.putText(img, str(int(fps)), (10, 70), cv2.FONT_HERSHEY_PLAIN, 3,
                    (255, 0, 255), 3)
 
        cv2.imshow("Image", img)
        cv2.waitKey(1)


        # add something
 
 
if __name__ == "__main__":
    main()