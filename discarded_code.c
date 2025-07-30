void wait(unsigned int ticks)
{   
  for (volatile unsigned int i = 0; i < ticks * 1000; i++);
} 

