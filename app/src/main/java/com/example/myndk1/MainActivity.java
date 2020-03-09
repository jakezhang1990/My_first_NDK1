package com.example.myndk1;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.TextView;

import org.fmod.FMOD;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");//lib + native-lib + .so
    }

    private String instanceField="java层实例变量";
    private static String staticField="java层静态变量";

    //任何一个方法在底层都对应了ArtMethod这样一个结构体，这个结构体就是热修复的核心,热修复就是修改这个结构体里面的成员。
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        TextView tv = findViewById(R.id.sample_text);
        tv.setText(stringFromJNI());//Hello from C++
        instance_MethodB();
        static_method();

        instanceMethodDynamic();
        staticMethodDynamic();

        getJavaField();

        callJavaMethod();


        FMOD.init(this);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        FMOD.close();
    }

    //    供JNI被动反射调用
    private String instanceMethod(){
        return "这是java层实例方法";
    }

    private static String staticMethod(){
        return "这是java层静态方法";
    }





    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    public native String instanceMethodA();

    public native void instance_MethodB();

    public static native void static_method();

    public native void instanceMethodDynamic();

    public static native void staticMethodDynamic();

    public native void getJavaField();//获取java层变量

    public native void callJavaMethod();//供jni调用java类的方法
}
