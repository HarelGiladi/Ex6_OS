#include <iostream>
#include <sys/mman.h>
#include <cassert>

//this code was wrriten with the help of the internet
#ifndef _SINGLETON_HH
#define _SINGLETON_HH


pthread_mutex_t locker;
template<typename T>

class Singleton {
    public:
        static Singleton<T>* instance();
        static void destroy();

    private:
        Singleton(Singleton const&){};
        Singleton& operator=(Singleton const&){};
        
        //this could also be wrriten in protected
        static Singleton<T>* m_instance;
        Singleton(){ m_instance = static_cast <Singleton<T>*> (this); };
        ~Singleton(){  };
};

    template<typename T>
    Singleton<T>* Singleton<T>::m_instance = nullptr;

    template<typename T>
    Singleton<T>* Singleton<T>::instance() {
        pthread_mutex_lock(&locker);
        if(!m_instance)
        {
            Singleton<T>::m_instance = new Singleton<T>();
        }
        pthread_mutex_unlock(&locker);
        return m_instance;
    }

    template<typename T>
    void Singleton<T>::destroy() {
    
        delete Singleton<T>::m_instance;
        Singleton<T>::m_instance = nullptr;
    }

#endif

int main () {
    
    std::cout << "starts testing:\n" << std::endl;
    Singleton<FILE*>* s1 = Singleton<FILE*>::instance();
    Singleton<FILE*>* s2 = Singleton<FILE*>::instance();
    std::cout << s1 << "\n" << std::endl;
    std::cout << s2 <<"\n"<<std::endl;
    assert(s1 == s2);
    std::cout << "!ASSERTION WAS A SUCCSES!\n" << std::endl;

    std::cout << "!ASSERTION WAS A SUCCSES!\n" << std::endl;
    s2->destroy;
    Singleton<FILE*>* s3 = Singleton<FILE*>::instance();
    std::cout << s3<<"\n" << '\n';
    assert(s1 == s3);
    std::cout << "!ASSERTION WAS A SUCCSES!\n" << std::endl;
    std::cout << "!ASSERTION WAS A SUCCSES!\n" << std::endl;

    std::cout <<  "!TEST PASSED SECCESSFULLY!\n" <<std::endl;

    return(0);

}
