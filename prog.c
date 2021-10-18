#include <pthread.h>
#include <stdio.h>
#include  <stdlib.h>
#define NB_THREADS	3



void *travailUtile(void *n)
{
   int i;
   double resultat=0.0;
   double val=*(double *)n;
   printf("val %f \n",val);
   for (i=0; i<1000000; i++)
   {
     resultat = resultat +val ;
   }
   printf("resultat = %e\n",resultat);
   pthread_exit((void *) 0);
}

int main (int argc, char *argv[])
{
   pthread_t thread[NB_THREADS];
   pthread_attr_t attr;
   int rc, t;
   void *status;
   size_t stacksize;
   /* Initialisation et activation dâ€™attributs */
   pthread_attr_init(&attr); //valeur par dÃ©faut
   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE); //attente du thread possible
   pthread_attr_getstacksize(&attr,&stacksize);
   printf("la taille de stacksize  est de  %ld\n", stacksize);
   printf("Modification de la taille de la pile...\n");
   size_t n_stacksize=stacksize +1000000;
   double tab[NB_THREADS];
   for (int i = 0; i < NB_THREADS; ++i)
   {
      tab[i]=i*i;
   }
   pthread_attr_setstacksize(&attr,n_stacksize);
   printf("la nouvelle taille de la pile est de : %ld\n", n_stacksize);
   for(t=0; t<NB_THREADS; t++)
   {
      printf("Creation du thread %d\n", t);
      rc = pthread_create(&thread[t], &attr, travailUtile, &tab[t]); 
      if (rc)
      {
         printf("ERROR; le code de retour de pthread_create() est %d\n", rc);
         exit(-1);
      }
   }

   /* liberation des attributs et attente de la terminaison des threads */
   pthread_attr_destroy(&attr);
   for(t=0; t<NB_THREADS; t++)
   {
      rc = pthread_join(thread[t], &status);
      if (rc)
      {
         printf("ERROR; le code de retour du pthread_join() est %d\n", rc);
         exit(-1);
      }
      printf("le join a fini avec le thread %d et a donne le status= %ld\n",t, (long)status);
   }

   pthread_exit(NULL);
}

