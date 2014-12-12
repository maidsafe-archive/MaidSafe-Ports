var nodejsPort = require('./nodejs_port');

var testClassObject = nodejsPort.TestClass.RawFactoryFunction();

var randomInt = Math.round((Math.random() * (1000 - 1)) + 1);

console.log('Random Int:', randomInt);

console.log('Passing var to C++');
testClassObject.SetMyValue(randomInt);

console.log('Getting var from C++:', testClassObject.GetMyValue());

console.log('Getting Random UInt from MS-Common>utils.h:', testClassObject.GetRandomUInt32())

console.log('\nPassing 86.05 to the function templates below:')
console.log('Getting int from function template:', testClassObject.GetTemplateValueAsInt(86.05))
console.log('Getting char from function template:', testClassObject.GetTemplateValueAsChar(86.05))
console.log('Getting double from function template:', testClassObject.GetTemplateValueAsDouble(86.05))

console.log('\nTestClass::PrintMe():', testClassObject.PrintMe());
testClassObject.PrintLogMessage();

console.log('\nDone...');
