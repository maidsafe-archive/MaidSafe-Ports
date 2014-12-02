package org.maidsafe;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.ScrollView;
import java.util.Random;
import android.text.method.ScrollingMovementMethod;

public class MyTest extends Activity
{
    TextView outputText = null;
    ScrollView scroller = null;

    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);

        outputText = (TextView)findViewById(R.id.OutputText);
        outputText.setText("Press 'Run' to start...\n");
        outputText.setMovementMethod(new ScrollingMovementMethod());

        scroller = (ScrollView)findViewById(R.id.Scroller);
    }

    public void onRunButtonClick(View view)
    {
      outputText.append("Started...\n");
      nativeCall();
      outputText.append("Finished!\n");
      
      // Ensure scroll to end of text
      scroller.post(new Runnable() {
        public void run() {
          scroller.fullScroll(ScrollView.FOCUS_DOWN);
        }
      });
    }

    /** Calls into C/C++ code */
    public void nativeCall()
    {
      outputText.append("Native Call...\n");
      TestClass testClassObject = TestClass.RawFactoryFunction();
      Random rnd = new Random();
      int iRandom = rnd.nextInt(1000) + 1;
      outputText.append("\nRandom Int: " + iRandom);
      outputText.append("\nPassing var to C++..");
      testClassObject.SetMyValue(iRandom);
      outputText.append("\nGetting var from C++: " + testClassObject.GetMyValue());
      //outputText.append("\nGetting Random var from C++: " + testClassObject.GetRandomUInt32());

      outputText.append("\n\nPassing data to the int, char and double function templates below:");
      outputText.append("\nGetting int from function template: " + testClassObject.GetTemplateValueAsInt(86));
      outputText.append("\nGetting char from function template: " + testClassObject.GetTemplateValueAsChar('V'));
      outputText.append("\nGetting double from function template: " + testClassObject.GetTemplateValueAsDouble(86.05));

      outputText.append("\n\nTestClass::PrintMe(): " + testClassObject.PrintMe());
      outputText.append("\n\nTestClass::Connection result: " + testClassObject.CheckBootstrapsConnectivity());
      outputText.append("\n\nDone...");
    }

    /** static constructor */
    static {
        System.loadLibrary("android_port");
    }
}
