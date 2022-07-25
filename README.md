# SERIALIZER
这是一个简单的序列化/反序列化库，可以用于上下位机通信，持久化存储等场景  
纯头文件库，提供了C/C++接口，

## 与protobuf相比
本程序采用静态反射的方式提供序列化功能，只需要包含头文件并定义并添加数个简单的宏定义，无需配置工程或手工生成代码，对嵌入式编程更加友好

## 快速开始
1. 将`pp.h`，`serialize.h`添加至工程依赖
2. （可选）在C语言上，我们还通过`stack.h`提供了变长数组格式化支持，若需要该功能请包含该头文件  
    注意：必须先包含`serialize.h`，再包含`stack.h`，才会自动生成反射
3. （可选）生成自定义类型（结构体、类等）反射  
    * 若该类型所有数据均存储于栈上（即没有包含指针类型的结构体类型及其他基本数据类型），请使用`SERIALIZE_OBJ_DEF`宏完成定义  
        ```c++
        typedef struct {
            int a;
            float b;
            char c;
        } test;
        SERIALIZE_OBJ_DEF(test)
        ```
    * 若不符合上述条件或只需要保存部分成员，请使用`SERIALIZE_DEF`宏完成定义  
        cpp示例：  
        ```c++
        typedef struct {
            int a;
            float b;
            char c;
        } test;
        SERIALIZE_DEF(test, a, b)
        ```
      c示例：  
        ```c
        typedef struct {
            int a;
            float b;
            char c;
        } test;
        SERIALIZE_DEF(test, (int, a), (float, b))
        ```
4. 序列化与反序列化数据  
     请分别参考`main.cpp`与`main.c`文件中的示例

## CPP API
#### 宏定义：SERIALIZE_OBJ_DEF(CTX)
参数 ctx： 类型名  
生成该类型基于内存拷贝的序列化与反序列化反射

#### 宏定义：SERIALIZE_DEF(CTX, member1, member2, ...)
参数 ctx： 类型名  
参数 memberX： 需要序列化的成员名  
生成该类型基于成员遍历的序列化与反序列化反射  
备注： 兼容C API中的SERIALIZE_DEF定义

#### 函数定义：SERIALIZE(const CTX& _in, std::string& buf)
参数 _in： 被序列化的对象（需要先生成对应反射）  
参数 buf： 保存序列化产物的字符串  
将输入的_in格式化，并追加保存到buf字符串

#### 函数定义：DESERIALIZE(CTX& _in, std::string& buf)
参数 _in： 反序列化产物（需要先生成对应反射）  
参数 buf： 序列化字符串  
从buf头部反序列化出_in对象并赋值，同时将从buf中删除已经反序列化的子串

## C API
#### 宏定义：SERIALIZE_OBJ_DEF(CTX)
参数 ctx： 类型名  
生成该类型基于内存拷贝的序列化与反序列化反射

#### 宏定义：SERIALIZE_DEF(CTX, (CTX1, member1), (CTX2, member2), ...)
参数 ctx： 类型名  
参数 CTXx： 需要序列化的成员类型  
参数 memberX： 需要序列化的成员名  
生成该类型基于成员遍历的序列化与反序列化反射  

#### 函数定义：SERIALIZE(CTX)(CTX* _in, uint8_t* buf, int* n)
参数 ctx： 类型名  
参数 _in： 被序列化的对象的指针（需要先生成对应反射）  
参数 buf： 保存序列化产物的字符串  
参数 n： 保存序列化字符串的有效长度  
将输入的_in格式化，并追加保存到buf字符串，同时通过参数n返回字符串中的有效长度

#### 函数定义：DESERIALIZE(CTX)(CTX* _in, uint8_t* buf, int* n)
参数 ctx： 类型名  
参数 _in： 反序列化产物（需要先生成对应反射）  
参数 buf： 已序列化字符串
参数 n： 将存储实际读取的字符数  
从buf头部反序列化出_in对象并赋值，同时通过参数n返回实际读取的字符串长度

## STACK API
#### 成员定义：data
指向数据存储位置的指针，在未init或deinit后为null

#### 成员定义：n
当前存储的数据长度

#### 成员定义：max
该stack最大可存储的长度

#### 宏定义：STACK_DEF(CTX)  
参数 ctx： 类型名  
生成该类型的STACK代码

#### 宏定义：STACK(CTX)
参数 ctx： 类型名  
返回该类型的stack的类型名

#### 函数定义：STACK_INIT(CTX, NUM)
参数 ctx： 类型名  
参数 num： 该stack最大存储容量  
返回： stack对象  
申请一片类型为ctx，大小为num的内存空间，并返回对应stack对象

#### 函数定义：STACK_DEINIT(CTX)(STACK(CTX)* _in)
参数 ctx： 类型名  
参数 _in： 指向要释放的stack对象的指针  
释放该stack对象所申请的内存

#### 函数定义：STACK_INSERT(CTX)(STACK(CTX)* _in, int n, ctx obj)
参数 ctx： 类型名  
参数 _in： 指向要操作的stack对象的指针  
参数 n： 要插入数据的位置
参数 obj： 要插入的对象  
将obj对象插入到下标n位置

#### 函数定义：STACK_PUSH(CTX)(STACK(CTX)* _in, ctx obj)
参数 ctx： 类型名  
参数 _in： 指向要操作的stack对象的指针  
参数 obj： 要插入的对象  
将obj对象插入到stack末尾


#### 函数定义：STACK_POP(CTX)(STACK(CTX)* _in, int n)
参数 ctx： 类型名  
参数 _in： 指向要操作的stack对象的指针  
参数 n： 要删除的元素下标  
返回值： 被删除的对象
将stack中下标n位置的元素删除，并返回被删除的元素的值

#### 序列化与反序列化
* 正常使用SERIALIZE(CTX)完成序列化
* DESERIALIZE时请传入未init的对象，程序将通过序列化结果自动init对应大小的stack

#### 其他
提供了类似cpp类的访问接口，如
```c
tt.deinit(&tt);
```
无需传入ctx类型，自动从结构体中推导

## 实现细节
* 在cpp上，通过重载、模板函数的方案实现serialize，已经自动添加常见数值类型以及vector/list/map的序列化与反序列化
* 在c语言上，为了保证兼容性，没有采用多态的方案，不同类型将分别生成对应的`SERIALIZE_CTX()`和`DESERIALIZE_CTX()`函数，然后通过`SERIALIZE(CTX)()`拼接调用
* 对于变长数组序列化，将首先序列化一个int型表示长度，然后依次序列化数组成员
* 对于成员遍历反射，目前还不支持定长数组，因为在代码生成的过程中无法获取数组长度用于遍历
* `pp.h`为宏定义预处理代码，该文件参考boost pp库完成
