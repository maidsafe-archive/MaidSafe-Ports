using System;

namespace Sample {
  internal class Program {
    private static void Main(string[] args) {
      var testClassObject = TestClass.RawFactoryFunction();

      var rnd = new Random();
      var randomInt = rnd.Next(1, 1000);
      Console.WriteLine("Random int: {0}", randomInt);
      Console.WriteLine("Passing value to C++");
      testClassObject.SetMyValue(randomInt);
      Console.WriteLine("Getting value from C++: {0}", testClassObject.GetMyValue());

      Console.WriteLine("Getting Random Int from MS-Common: {0}", testClassObject.GetRandomUInt32());
      
      Console.WriteLine("Getting int from function template: {0}", testClassObject.GetTemplateValueAsInt(86));
      Console.WriteLine("Getting char from function template: {0}", testClassObject.GetTemplateValueAsChar('8'));
      Console.WriteLine("Getting double from function template: {0}", testClassObject.GetTemplateValueAsDouble(86.05));

      Console.WriteLine("\nTestClass::PrintMe(): {0}", testClassObject.PrintMe());
      
      Console.Read();
    }
  }
}
