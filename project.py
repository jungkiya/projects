# actions.py
import time, serial
import cv2
import numpy as np


# 메세지 형식 10바이트 고정
def msg_10byte(message):
    """
    주어진 메시지를 10바이트 길이로 고정하여 반환합니다.
    Args:
        message (str): 전송할 메시지
    Returns:
        bytes: 10바이트 길이의 메시지
    """
    # 메시지를 바이트 문자열로 변환
    msg_bytes = message.encode()

    # 메시지 길이가 10바이트가 되도록 패딩
    if len(msg_bytes) < 10:
        msg_bytes = msg_bytes.ljust(10, b" ")
    elif len(msg_bytes) > 10:
        msg_bytes = msg_bytes[:10]

    return msg_bytes


py_serial = serial.Serial(
    ## 컴퓨터로 연결시 port = "COM@" 사용
    # port="COM3",
    ## 라즈베리파이로 연결시 "/dev/ttyACM@" 사용
    "/dev/ttyUSB0",
    # 보드 레이트 (통신 속도)
    baudrate=9600,
)


# 컨베이어 벨트 이동 명령 (Start)
def Move(client_socket):
    print("Move function called.")
    commend = ord("a")
    py_serial.write(bytes([commend]))

    time.sleep(4)
    client_socket.sendall(msg_10byte("Move_OK"))
    time.sleep(1)
    print("Move function complete")


# 카메라 촬상 명령
def Capture(client_socket):
    print("Capture function called.")

    capture = cv2.VideoCapture(0)
    ret, frame = capture.read()

    # 카메라 영상 읽기 실패 처리
    if not ret:
        raise Exception("Failed to capture image")
    client_socket.sendall(msg_10byte("Cam_OK"))

    time.sleep(2)

    cv2.imwrite("/home/pi/images/captured.png", frame)
    image = cv2.imread("/home/pi/images/captured.png")
    print("Capture function complete.")
    client_socket.sendall(msg_10byte("Film_OK"))
    return image


# 검출 후 불량일 때, 컨베이어 벨트 되돌아가기
def Reverse():
    print("Reverse function called.")
    commend = ord("b")
    py_serial.write(bytes([commend]))

    time.sleep(4)
    print("Reverse function complete")


# 검출 후 정상일 때, 분류 통에 들어가기
# 0-1000원 1-5000원 2-10000원 3-50000원
def OK(n):
    print(f"OK function called with value {n}.")
    # 여기에 OK 동작 코드 추가
    if n == 0:
        print("Performing action for 0 (1000원).")
        commend = ord("c")
        py_serial.write(bytes([commend]))
        # 0번 동작 시뮬레이션
        # if response.decode().strip() == "3":
        #    print("1000won complete")
    elif n == 1:
        print("Performing action for 1 (5000원).")
        commend = ord("e")
        py_serial.write(bytes([commend]))
        # 1번 동작 시뮬레이션
        # if response.decode().strip() == "5":
        #    print("5000won complete")
    elif n == 2:
        print("Performing action for 2 (10000원).")
        commend = ord("f")
        py_serial.write(bytes([commend]))
        # 2번 동작 시뮬레이션
        # if response.decode().strip() == "6":
        #    print("10000won complete")
    elif n == 3:
        print("Performing action for 3 (50000원).")
        commend = ord("g")
        py_serial.write(bytes([commend]))
        # 3번 동작 시뮬레이션
        # if response.decode().strip() == "7":
        #    print("50000won complete")
    else:
        print(f"Unknown action for {n}.")
        # 기본 동작 추가
        # 기본 동작 시뮬레이션
