var nodejsPort = require('./nodejs_port');

var testClassObject = nodejsPort.TestClass.RawFactoryFunction();

var randomInt = Math.round((Math.random() * (1000 - 1)) + 1);

console.log('Random Int: ' + randomInt);

console.log('Passing var to C++');
testClassObject.SetMyValue(randomInt);

console.log('Getting var from C++: ' + testClassObject.GetMyValue());
console.log('Getting Static var from C++: ' + testClassObject.GetStaticConstValue());
console.log('\nDone...');
