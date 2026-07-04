/*
 * BĂ i 1 - Má»¥c 5.5: MĂ´ phá»ng tĂ­nh ans tá»« x1..x6 báº±ng cĂ¡c thread cháº¡y song song,
  * Ä‘á»“ng bá»™ hĂ³a báº±ng semaphore (POSIX semaphore, sem_t) trĂªn Linux.
   *
    * Äá»“ thá»‹ phá»¥ thuá»™c:
     *   (a) w = x1*x2          -> khĂ´ng phá»¥ thuá»™c gĂ¬
      *   (b) v = x3*x4          -> khĂ´ng phá»¥ thuá»™c gĂ¬
       *   (c) y = v*x5           -> cáº§n v (tá»« b)
        *   (d) z = v*x6           -> cáº§n v (tá»« b)
         *   (e) y = w*y            -> cáº§n w (tá»« a) vĂ  y (tá»« c)
          *   (f) z = w*z            -> cáº§n w (tá»« a) vĂ  z (tá»« d)
           *   (g) ans = y+z          -> cáº§n y (tá»« e) vĂ  z (tá»« f)
            *
             * BiĂªn dá»‹ch:  gcc -o sync_ans sync_ans.c -lpthread
              * Cháº¡y:       ./sync_ans
               */
               
               #include <stdio.h>
               #include <stdlib.h>
               #include <pthread.h>
               #include <semaphore.h>
               
               /* Dá»¯ liá»‡u Ä‘áº§u vĂ o */
               static int x1 = 2, x2 = 3, x3 = 4, x4 = 5, x5 = 6, x6 = 7;
               
               /* Biáº¿n trung gian vĂ  káº¿t quáº£ (chia sáº» giá»¯a cĂ¡c thread) */
               static int w, v, y, z, ans;
               
               /*
                * Semaphore dĂ¹ng Ä‘á»ƒ bĂ¡o hiá»‡u "dá»¯ liá»‡u Ä‘Ă£ sáºµn sĂ ng":
                 *  - sem_v   : v Ä‘Ă£ Ä‘Æ°á»£c tĂ­nh xong (b bĂ¡o cho c vĂ  d -> post 2 láº§n)
                  *  - sem_w   : w Ä‘Ă£ Ä‘Æ°á»£c tĂ­nh xong (a bĂ¡o cho e vĂ  f -> post 2 láº§n)
                   *  - sem_y_c : y (do c tĂ­nh) Ä‘Ă£ sáºµn sĂ ng cho e
                    *  - sem_z_d : z (do d tĂ­nh) Ä‘Ă£ sáºµn sĂ ng cho f
                     *  - sem_y_e : y (do e tĂ­nh) Ä‘Ă£ sáºµn sĂ ng cho g
                      *  - sem_z_f : z (do f tĂ­nh) Ä‘Ă£ sáºµn sĂ ng cho g
                       */
                       static sem_t sem_v, sem_w, sem_y_c, sem_z_d, sem_y_e, sem_z_f;
                       
                       void *task_a(void *arg) {
                           w = x1 * x2;
                               printf("(a) w = x1*x2 = %d\n", w);
                                   sem_post(&sem_w);   /* bĂ¡o cho (e) */
                                       sem_post(&sem_w);   /* bĂ¡o cho (f) */
                                           return NULL;
                                           }
                                           
                                           void *task_b(void *arg) {
                                               v = x3 * x4;
                                                   printf("(b) v = x3*x4 = %d\n", v);
                                                       sem_post(&sem_v);   /* bĂ¡o cho (c) */
                                                           sem_post(&sem_v);   /* bĂ¡o cho (d) */
                                                               return NULL;
                                                               }
                                                               
                                                               void *task_c(void *arg) {
                                                                   sem_wait(&sem_v);            /* chá» v tá»« (b) */
                                                                       y = v * x5;
                                                                           printf("(c) y = v*x5 = %d\n", y);
                                                                               sem_post(&sem_y_c);          /* bĂ¡o cho (e) */
                                                                                   return NULL;
                                                                                   }
                                                                                   
                                                                                   void *task_d(void *arg) {
                                                                                       sem_wait(&sem_v);            /* chá» v tá»« (b) */
                                                                                           z = v * x6;
                                                                                               printf("(d) z = v*x6 = %d\n", z);
                                                                                                   sem_post(&sem_z_d);          /* bĂ¡o cho (f) */
                                                                                                       return NULL;
                                                                                                       }
                                                                                                       
                                                                                                       void *task_e(void *arg) {
                                                                                                           sem_wait(&sem_w);            /* chá» w tá»« (a) */
                                                                                                               sem_wait(&sem_y_c);          /* chá» y tá»« (c) */
                                                                                                                   y = w * y;
                                                                                                                       printf("(e) y = w*y = %d\n", y);
                                                                                                                           sem_post(&sem_y_e);          /* bĂ¡o cho (g) */
                                                                                                                               return NULL;
                                                                                                                               }
                                                                                                                               
                                                                                                                               void *task_f(void *arg) {
                                                                                                                                   sem_wait(&sem_w);            /* chá» w tá»« (a) */
                                                                                                                                       sem_wait(&sem_z_d);          /* chá» z tá»« (d) */
                                                                                                                                           z = w * z;
                                                                                                                                               printf("(f) z = w*z = %d\n", z);
                                                                                                                                                   sem_post(&sem_z_f);          /* bĂ¡o cho (g) */
                                                                                                                                                       return NULL;
                                                                                                                                                       }
                                                                                                                                                       
                                                                                                                                                       void *task_g(void *arg) {
                                                                                                                                                           sem_wait(&sem_y_e);          /* chá» y tá»« (e) */
                                                                                                                                                               sem_wait(&sem_z_f);          /* chá» z tá»« (f) */
                                                                                                                                                                   ans = y + z;
                                                                                                                                                                       printf("(g) ans = y+z = %d\n", ans);
                                                                                                                                                                           return NULL;
                                                                                                                                                                           }
                                                                                                                                                                           
                                                                                                                                                                           int main(void) {
                                                                                                                                                                               pthread_t ta, tb, tc, td, te, tf, tg;
                                                                                                                                                                               
                                                                                                                                                                                   /* Khá»Ÿi táº¡o semaphore, giĂ¡ trá»‹ Ä‘áº§u = 0 (chÆ°a cĂ³ dá»¯ liá»‡u),
                                                                                                                                                                                          tham sá»‘ thá»© 2 = 0 nghÄ©a lĂ  chá»‰ chia sáº» giá»¯a cĂ¡c thread trong cĂ¹ng 1 tiáº¿n trĂ¬nh */
                                                                                                                                                                                              sem_init(&sem_v,   0, 0);
                                                                                                                                                                                                  sem_init(&sem_w,   0, 0);
                                                                                                                                                                                                      sem_init(&sem_y_c, 0, 0);
                                                                                                                                                                                                          sem_init(&sem_z_d, 0, 0);
                                                                                                                                                                                                              sem_init(&sem_y_e, 0, 0);
                                                                                                                                                                                                                  sem_init(&sem_z_f, 0, 0);
                                                                                                                                                                                                                  
                                                                                                                                                                                                                      /* Táº¡o 7 thread cháº¡y song song, thá»© tá»± thá»±c thi thá»±c sá»± do semaphore quyáº¿t Ä‘á»‹nh */
                                                                                                                                                                                                                          pthread_create(&ta, NULL, task_a, NULL);
                                                                                                                                                                                                                              pthread_create(&tb, NULL, task_b, NULL);
                                                                                                                                                                                                                                  pthread_create(&tc, NULL, task_c, NULL);
                                                                                                                                                                                                                                      pthread_create(&td, NULL, task_d, NULL);
                                                                                                                                                                                                                                          pthread_create(&te, NULL, task_e, NULL);
                                                                                                                                                                                                                                              pthread_create(&tf, NULL, task_f, NULL);
                                                                                                                                                                                                                                                  pthread_create(&tg, NULL, task_g, NULL);
                                                                                                                                                                                                                                                  
                                                                                                                                                                                                                                                      /* Chá» táº¥t cáº£ thread káº¿t thĂºc */
                                                                                                                                                                                                                                                          pthread_join(ta, NULL);
                                                                                                                                                                                                                                                              pthread_join(tb, NULL);
                                                                                                                                                                                                                                                                  pthread_join(tc, NULL);
                                                                                                                                                                                                                                                                      pthread_join(td, NULL);
                                                                                                                                                                                                                                                                          pthread_join(te, NULL);
                                                                                                                                                                                                                                                                              pthread_join(tf, NULL);
                                                                                                                                                                                                                                                                                  pthread_join(tg, NULL);
                                                                                                                                                                                                                                                                                  
                                                                                                                                                                                                                                                                                      printf("\nKet qua cuoi cung: ans = %d\n", ans);
                                                                                                                                                                                                                                                                                      
                                                                                                                                                                                                                                                                                          /* Giáº£i phĂ³ng semaphore */
                                                                                                                                                                                                                                                                                              sem_destroy(&sem_v);
                                                                                                                                                                                                                                                                                                  sem_destroy(&sem_w);
                                                                                                                                                                                                                                                                                                      sem_destroy(&sem_y_c);
                                                                                                                                                                                                                                                                                                          sem_destroy(&sem_z_d);
                                                                                                                                                                                                                                                                                                              sem_destroy(&sem_y_e);
                                                                                                                                                                                                                                                                                                                  sem_destroy(&sem_z_f);
                                                                                                                                                                                                                                                                                                                  
                                                                                                                                                                                                                                                                                                                      return 0;
                                                                                                                                                                                                                                                                                                                      }*/