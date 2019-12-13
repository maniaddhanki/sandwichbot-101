#ifndef SRC_UTILS_H_
#define SRC_UTILS_H_
#include <Arduino.h>
enum PostStatus
{
  NOT_DETECTED,
  RIGHT,
  LEFT
};

struct PostDetails
{
  int width;
  PostStatus status;
  long postStartingSteps;
};

struct Post
{
    PostDetails details;
    struct Post *next;
};

class Utils
{
public:
    static char readNextCharacterFromSerial1();
    static byte readNextByteFromSerial1();
    static void readNBytesFromSerial1(uint8_t noOfBytes, uint8_t byteArray[]);
    static void copyByteArrayElements(uint8_t array1[], uint8_t array2[], uint8_t numberOfElements);
};

#endif /* SRC_UTILS_H_ */