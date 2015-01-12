package org.maidsafe;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.ScrollView;
import java.util.Random;
import android.text.method.ScrollingMovementMethod;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.Toast;
import android.app.AlertDialog;

public class MyTest extends Activity
{
    TextView outputText = null;
    ScrollView scroller = null;
    private String[] apiCalls = new String[] {"Random", "Set and Get val", "Invoke templates", "Invoke PrintMe", "Check Rudp", "Print Log", "Http"};

    TestClass testClassObject = TestClass.RawFactoryFunction();
    Random rnd = new Random();
    int iRandom = rnd.nextInt(1000) + 1;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
      super.onCreate(savedInstanceState);

      testClassObject.InitialiseLogging();

      ArrayAdapter<String> adapter = new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1, apiCalls);
      setContentView(R.layout.main);
      ListView listView = (ListView) findViewById(R.id.listView);
      listView.setAdapter(adapter);
      listView.setOnItemClickListener(new OnItemClickListener() {
        @Override
        public void onItemClick(AdapterView<?> arg0, View arg1, int position, long arg3) {
            executeApi(position);
        }
      });
    }

    private void showToast(String message) {
        AlertDialog.Builder builder = new AlertDialog.Builder(this);
        builder.setMessage(message);
        builder.show();
    }

    private void executeApi(int id) {
      switch(id) {
        case 0:
          showToast("Random Int: " + iRandom);
          break;
        case 1:
          testClassObject.SetMyValue(iRandom);
          showToast("Getting var from C++: " + testClassObject.GetMyValue());
          break;
        case 2:
          String temp = "\nGetting int from function template: " + testClassObject.GetTemplateValueAsInt(86);
          temp += "\nGetting char from function template: " + testClassObject.GetTemplateValueAsChar('V');
          temp += "\nGetting double from function template: " + testClassObject.GetTemplateValueAsDouble(86.05);
          showToast(temp);
          break;
        case 3:
          showToast("\nTestClass::PrintMe(): " + testClassObject.PrintMe());
          break;
        case 4:
          showToast("\nTestClass::Connection result: " + testClassObject.CheckBootstrapsConnectivity());
          break;
        case 5:
          testClassObject.PrintLogMessage();
          showToast("\nLog Message Submitted.");
          break;
        case 6:
          showToast("Http Message:\n" + testClassObject.GetHttpMessage("192.168.92.130", "8080"));
          break;
        default:
          showToast("NO match found");
          break;
      }
    }

    /** static constructor */
    static {
        System.loadLibrary("crystax");
        System.loadLibrary("android_port");
    }
}
