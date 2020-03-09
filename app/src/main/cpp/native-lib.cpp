#include <jni.h>
#include <string>

#include <android/log.h>

#define LOG_TAG "JAKE_SEP"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)


// C++中编译C程序的说明


///////////////////////////////////////静态注册/////////////////////////////////////////

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_myndk1_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_myndk1_MainActivity_instanceMethodA(JNIEnv *env, jobject thiz) {
    // TODO: implement instanceMethodA()
    std::string hello = "Hello world ndk";
    return env->NewStringUTF(hello.c_str());
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_myndk1_MainActivity_instance_1MethodB(JNIEnv *env, jobject thiz) {
    // TODO: implement instance_MethodB()
    LOGE("实例注册");
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_myndk1_MainActivity_static_1method(JNIEnv *env, jclass clazz) {
    // TODO: implement static_method()
    LOGE("静态注册");
}

///////////////////////////静态注册/////////////////////////////////////////


/////////////////////////动态注册///////////////////////////////////////////


//int registerNatives();//声明

int registerNativeMethods(JNIEnv *env, const char *className, int nMethods) ;


/*
 * typedef struct {
    const char* name;//java方法名
    const char* signature;//java方法的签名
    void*       fnPtr;//JNI中对应的方法指针
} JNINativeMethod;
 * */

/**
 * 【第一步 1.2】
 * 这是Native层真正实现的方法，真正调用的实例方法
 */
void instanceMethodDynamic(){
    LOGE("动态注册 instanceMethodDynamic 方法");
}

void staticMethodDynamic(){
    LOGE("动态注册 staticMethodDynamic 方法");
}

/**
 * 【第一步 1.1】
 * 本方法是表示：需要注册的函数列表。
 *
 * 方法的结构体里面数组的元素是JNINativeMethod结构体，
 * 这个结构体的第一个参数：java方法名，第二个参数，方法签名，
 * 第三个参数：方法的指针，
 * 这个指针参数怎么获取呢？把上面定义的Native层的真正实现的方法名拷贝过来，使用alt+enter提示
 * 类型转换，生成的。
 *
 * 后续如果有新增加的方法，在这个【步骤 1.1】数组结构体中增加对应方法的注册。其次在【第一步 1.2】中实现具体的方法内容即可。
 */
JNINativeMethod jniNativeMethods[]={
        {"instanceMethodDynamic","()V", (void *)(instanceMethodDynamic)},
        {"staticMethodDynamic","()V",   (void *)(staticMethodDynamic)}
};


/**
 * 如果这个方法调用在这个方法实现之前，就需要在使用之前先声明，如果不声明,
 *  就需要把方法的实现写在调用之前，因为C语言是逐行去读取的。
 * 【第二步】
 */
int registerNatives( JNIEnv *env) {
    const char *className="com/example/myndk1/MainActivity";//包名的点要用反斜杠替换，这就是类名className
    return registerNativeMethods(env,className, sizeof(jniNativeMethods)/ sizeof(jniNativeMethods[0]));
}

//【第三步】
int registerNativeMethods(JNIEnv *env, const char *className, int nMethods) {

    jclass clazz = env->FindClass(className);
    /**
     * RegisterNatives(jclass clazz, const JNINativeMethod* methods,jint nMethods)
     * jclass clazz,这个方法所在的类的Class对象
     * const JNINativeMethod* methods, 结构体的指针
     * jint nMethods 注册的方法个数
     */
    if (env->RegisterNatives(clazz, jniNativeMethods, nMethods) <0){//注册
        return JNI_FALSE;
    }
    return JNI_TRUE;
}

//System.LoadLibrary时，第一次执行的回调函数，【第一步 1.0】
jint JNI_OnLoad(JavaVM *vm, void *reserved){

    JNIEnv *env=NULL;//一级指针

    vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6);//第一个参数是二级指针
    //二级指针作为参数，一般会用来干嘛呢？
    // 会在函数内部对这个二级指针所对应的一级指针进行一个初始化赋值，也就是说，在函数内部能够改变这个二级指针对应的一级指针的指向。
    //因为二级指针保存的是一级指针的地址。

    //动态注册
    if (!registerNatives(env)){
        return -1;
    }//注册本地方法
    return JNI_VERSION_1_6;
}

//////////////////////////////////动态注册///////////////////////////////////////

extern "C" JNIEXPORT void JNICALL
Java_com_example_myndk1_MainActivity_getJavaField(
        JNIEnv* env,
        jobject  thiz) {
    //在这里获取 MainActivity 中的 instanceField、staticField 属性内容。

    /**
     * jobject GetObjectField(jobject obj, jfieldID fieldID)
     * 第一个参数表示这个属性是属于哪个类对象，一般都是当前对象，也就是当前jobject。
     * 第二个参数是属性ID.通过env->GetFieldID()或者env->GetStaticFieldID()获取。
     *
     * jfieldID GetFieldID(jclass clazz, const char* name, const char* sig)
     * jfieldID GetStaticFieldID(jclass clazz, const char* name, const char* sig)
     * 第一个参数是jclass，也就是当前这个对象的Class，可以通过env->FindClass()获取到
     * 第二个参数是变量名。
     * 第三个参数是变量的签名
     *
     *
     */
    //成员变量的获取
    jclass clazz=env->FindClass("com/example/myndk1/MainActivity");
    jfieldID instanceFieldID=env->GetFieldID(clazz,"instanceField","Ljava/lang/String;");

    jstring instanceField= static_cast<jstring>(env->GetObjectField(thiz, instanceFieldID));//这就拿到了这个jstring类型的变量
    //将jstring转换成C,C++的字符串，调用打印方法，进行打印输出。
    const char *instanceRes= env->GetStringUTFChars(instanceField,/*JNI_FALSE*/NULL);
    LOGE("静态注册 instanceField: %s",instanceRes);//--->"java层实例变量"

    //静态变量的获取
    jfieldID staticFieldID=env->GetStaticFieldID(clazz,"staticField","Ljava/lang/String;");

    jstring staticField= static_cast<jstring>(env->GetStaticObjectField(clazz, staticFieldID));//这就拿到了这个jstring类型的变量
    //将jstring转换成C,C++的字符串，调用打印方法，进行打印输出。
    const char *staticRes= env->GetStringUTFChars(instanceField,/*JNI_FALSE*/NULL);
    LOGE("静态注册 instanceField: %s",staticRes);//--->"java层静态变量"
}


extern "C" JNIEXPORT void JNICALL
Java_com_example_myndk1_MainActivity_callJavaMethod(
        JNIEnv* env,
        jobject  thiz) {//JNI方法的反射调用，反射方式调用private String instanceMethod()和private static String staticMethod()方法。
    jclass clazz=env->FindClass("com/example/myndk1/MainActivity");
    //任何一个方法在底层都对应了ArtMethod这样一个结构体，这个结构体就是热修复的核心,热修复就是修改这个结构体里面的成员。
    //所以，每一个jmethodID都对应了一个ArtMethod结构体。
    jmethodID instanceJmethodId=env->GetMethodID(clazz,"instanceMethod","()Ljava/lang/String;");
    jobject instanceObj=env->CallObjectMethod(thiz,instanceJmethodId);//方法返回的是jobject，但是其实是jstring的，强转即可
    jstring instanceStr= static_cast<jstring>(instanceObj);//打印之前需要将字符串转换
    const char *instanceStrRes=env->GetStringUTFChars(instanceStr,NULL);
    LOGE("静态注册 实例方法instanceMethod：,%s",instanceStrRes);//--->"这是java层实例方法"


    jmethodID staticJmethodId=env->GetStaticMethodID(clazz,"staticMethod","()Ljava/lang/String;");
    jobject staticObj=env->CallStaticObjectMethod(clazz,staticJmethodId);//方法返回的是jobject，但是其实是jstring的，强转即可
    jstring staticStr= static_cast<jstring>(staticObj);//打印之前需要将字符串转换
    const char *staticStrRes=env->GetStringUTFChars(staticStr,NULL);
    LOGE("静态注册 静态方法staticMethod：,%s",staticStrRes);//-->"这是java层静态方法";
}
