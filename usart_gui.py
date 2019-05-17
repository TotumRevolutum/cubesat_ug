import sys
import time

f = open('USART_data.txt', 'a')
f.write("START")
data_len = 5
inp = ""
data = dict.fromkeys(['packageNum', 'x', 'y', 'z', 'time'])
while 1:
    inp = input()
    if inp == 'exit':
        print("CLOSE")
        f.close()
        exit()
    else:
        out = ''
        # time.sleep(170)
        for i in range(data_len):
            out += format(ord(sys.stdin.read(1)), "08b")
        if out != '':
            data['packageNum'] = int(out[0:16], 2)
            print("READED DATA")
            print(">>" + out)
            print(data['packageNum'])
            # + other variables
            f.write(out + '\n')

# input=1
# while 1 :
#     # get keyboard input
#     input = input(">> ")
#     if input == 'exit':
#         ser.close()
#         exit()
#     else:
#         # send the character to the device
#         # (note that I happend a \r\n carriage return and line feed to the characters - this is requested by my device)
#         ser.write(input + '\r\n')
#         out = ''
#         # let's wait one second before reading output (let's give device time to answer)
#         time.sleep(1)
#         while ser.inWaiting() > 0:
#             out += ser.read(1)
#
#         if out != '':
#             print (">>" + out)
