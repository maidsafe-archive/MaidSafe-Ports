import python_port
import random

testClassObject = python_port.TestClass.RawFactoryFunction()
randomInt = random.randint(1, 1000)
print "{} {}".format("Random Int:", randomInt)
print "Passing var to C++"
testClassObject.SetMyValue(randomInt)
print "{} {}".format("Getting var from C++:", testClassObject.GetMyValue())
print "{} {}".format("Getting Random UInt from MS-Common>utils.h:", testClassObject.GetRandomUInt32())

print "\nPassing 86 to the int & char function templates and 86.05 to the double function template below:"
print "{} {}".format("Getting int from function template:", testClassObject.GetTemplateValueAsInt(86))
print "{} {}".format("Getting char from function template:", testClassObject.GetTemplateValueAsChar(86))
print "{} {}".format("Getting double from function template:", testClassObject.GetTemplateValueAsDouble(86.05))

print "{} {}".format("\nTestClass::PrintMe():", testClassObject.PrintMe())

print "\nEncoding - Some String"
hexEncodedString = testClassObject.HexEncode("Some String")
print "{} {}".format("Hex:", hexEncodedString)
print "{} {}".format("Decoded:", testClassObject.HexDecode(hexEncodedString))
print "\nDone..."
