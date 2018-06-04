#include <pthread.h>
#include <unistd.h>
#include <stdio.h>

pthread_mutex_t _mutex;
int _seconds, _minutes, _hours;

void print_clock(int hours, int minutes, int seconds)
{
  printf("%d%d:%d%d:%d%d\n", hours/10, hours%10, minutes/10,
                             minutes%10, seconds/10, seconds%10);
}

void *clock_thread(void* unused)
{
  while(1)
  {
    sleep(1);

    pthread_mutex_lock(&_mutex);
    ++_seconds;
    if(_seconds > 59){
      _seconds = 0;
      ++_minutes;
    }
    if(_minutes > 59){
      _minutes = 0;
      ++_hours;
    }
    if(_hours > 23){
      _hours = 0;
    }
    print_clock(_hours, _minutes, _seconds);
    pthread_mutex_unlock(&_mutex);
  }
}

void *set_clock_thread(void *unused)
{
  while(1)
  {
    int seconds, minutes, hours;
    scanf("%d %d %d",&seconds, &minutes, &hours);

    if(seconds < 60 && seconds >= 0 &&
       minutes < 60 && minutes >= 0 &&
       hours < 24 && hours >= 0){

      pthread_mutex_lock(&_mutex);
      _seconds = seconds;
      _minutes = minutes;
      _hours = hours;
      print_clock(_hours, _minutes, _seconds);
      pthread_mutex_unlock(&_mutex);
    }
  }
}





int main()
{
  pthread_mutex_init(&_mutex, NULL);
  _seconds = 49;
  _minutes = 59;
  _hours = 23;

  pthread_t clock_thread_handle;
  pthread_t set_thread_handle;

  pthread_create(&clock_thread_handle, NULL, clock_thread, 0);
  pthread_create(&set_thread_handle, NULL, set_clock_thread, 0);

  pthread_join(clock_thread_handle, NULL);
  pthread_join(set_thread_handle, NULL);
}
