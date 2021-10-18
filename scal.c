    #include <pthread.h>
    #include <stdio.h>
    #include <stdlib.h>
    #define VECTOR_LENGTH 400
    #define NB_THREADS	5

    int vec1[VECTOR_LENGTH]; //Vector 1
    int vec2[VECTOR_LENGTH]; //Vector 2
    int res;
    int cpt=0;
    pthread_cond_t cond;
    pthread_mutex_t verrou;

    void *computeDotProduct(void * n)
    {
        
        int i;
        int resultat=0;
        int val=*(int *)n;
        printf("indice du thread %d \n",val);
        for (i=0; i<99; i++)
        {
            resultat +=vec2[(val+100)+i]*vec1[(val+100)+i] ;
        }
        printf("resultat = %d\n",resultat);
        pthread_mutex_lock(&verrou); /* DEBUT SECTION CRITIQUE */
        res+=resultat;
        cpt++;
        if (cpt==NB_THREADS-1)
        {
            pthread_cond_signal(&cond);
        }
        pthread_mutex_unlock(&verrou);
        pthread_exit((void *) 0);
    }

    
    void *affichage(void * n)
    {
        pthread_mutex_lock(&verrou);
        if (cpt<VECTOR_LENGTH)
        {
            pthread_cond_wait(&cond, &verrou);
        }
        printf("La valeur du resultat final est %d\n",res );
        pthread_mutex_unlock(&verrou);
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
        pthread_mutex_init(&verrou, NULL);
        pthread_cond_init(&cond,0);
        pthread_attr_init(&attr); 
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE); //attente du thread possible
        //Initialise vec1 and vec2
        for(t=0;t<VECTOR_LENGTH;t++){
            vec1[t] = 2;
            vec2[t] = 5;
        }  
        int tab[NB_THREADS];
        for (int i = 0; i < NB_THREADS; ++i)
        {
            tab[i]=i;
        }
        for(t=0; t<NB_THREADS-1; t++)
        {
            printf("Creation du thread %d\n", t);
            rc = pthread_create(&thread[t], &attr, computeDotProduct, &tab[t]); 
            if (rc)
            {
                printf("ERROR; le code de retour de pthread_create() est %d\n", rc);
                exit(-1);
            }
        }
        /*--------------------------------------------------------------------------------*/
        printf("Creation du thread d'affichage \n");
        rc = pthread_create(&thread[NB_THREADS-1], &attr, affichage,NULL); 
        if (rc)
        {
            printf("ERROR; le code de retour de pthread_create() est %d\n", rc);
            exit(-1);
        }
        /*--------------------------------------------------------------------------------*/
        //Remove threads
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
            //printf("le join a fini avec le thread %d et a donne le status= %ld\n",t, (long)status);
        }
        //printf("La valeur de res est ... %d \n",res );
        pthread_cond_destroy(&cond);
        pthread_mutex_destroy(&verrou);

   pthread_exit(NULL);
}
