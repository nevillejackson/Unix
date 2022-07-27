/*
 * program mooresfolli  
 * (*deterministic model for founder cell population growth
 *  and follicle initiation*) 
 *
 * this version assumes 2 types of sites [P & So] either all available at
 * [pstarttime & sostarttime] or demand never exceeds supply until total
 * number exhausted.
 *
 */

#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#include	"folli.h"

const int ztime=60; 
const int  maxtime=300;
const int  timeincrement=1;
const double  surfareaconst=9.0;
const double  adultweight=40000;

main(int argc,char *argv[])
{
  int time;
  int interval,pstarttime,pinterval,pendtime,sostarttime,soendtime
    ,sointerval,ptime,sotime,sdtime;
  double weight,growthint,surfarea, growthrate,cellbirthprob,follinitrate
    ,pavecellno,soavecellno,sdavecellno
      ,zcellno,psiteno,sositeno,sopratio
	,dens,pdens,sodens,sddens,sdens,ratio;
  double *cellno,*follno,*diffcellno,*pfollno,*sofollno,*sdfollno;
	/* timearray [ztime..maxtime] */
  
/* enter parameters */
  printf(" number of primary sites ");
  scanf("%lf",&psiteno); 
  printf("%5.0lf\n",psiteno);
  printf(" So/P ratio ");
  scanf("%lf",&sopratio);
  printf("%6.2lf\n",sopratio);
  printf(" growthrate - slope of log_wt/log_age line "); 
  scanf("%lf",&growthrate);
  printf("%8.5lf\n",growthrate); 
  printf(" growth intercept - of log_wt/log_age line ");
  scanf("%lf",&growthint);
  printf("%12.10lf\n",growthint);
  printf(" follicle initiation rate - increase per timeincrement per cm sq ");
  scanf("%lf",&follinitrate); 
  printf("%8.4lf\n",follinitrate);
  printf(" time of start of primary follicle initiation period "); 
  scanf("%d",&pstarttime);
  printf("%8d\n",pstarttime); 
  printf(" time of start of secondary original follicle initiation period "); 
  scanf("%d",&sostarttime);
  printf("%8d\n",sostarttime); 
  printf(" number of founder cells at time zero "); 
  scanf("%lf",&zcellno);
  printf("%8.1lf\n",zcellno); 
  printf(" average number of cells per p,so,sd follicle"); 
  scanf("%lf %lf %lf ",&pavecellno,&soavecellno,&sdavecellno); 
  printf("%8.3lf %8.3lf %8.3lf \n",pavecellno,soavecellno,sdavecellno);
  
/* malloc array space */
  pfollno = (double*)(malloc((size_t)(maxtime+1)*sizeof(double)));
  sofollno = (double*)(malloc((size_t)(maxtime+1)*sizeof(double)));
  sdfollno = (double*)(malloc((size_t)(maxtime+1)*sizeof(double)));
  cellno = (double*)(malloc((size_t)(maxtime+1)*sizeof(double)));
  follno = (double*)(malloc((size_t)(maxtime+1)*sizeof(double)));
  diffcellno = (double*)(malloc((size_t)(maxtime+1)*sizeof(double)));

/* initialize variables before growth loop */
  printf("\n\n");
  printf(" time     foll  pfoll   sofoll   sdfoll diffoundcel  foundcel"
  "  wght surfarea folirate celbrate    d    pd   sod   sdd    sd  rat\n");
  time = ztime ;
  pfollno[ztime] = 0.0;
  sofollno[ztime] = 0.0;
  sdfollno[ztime] = 0.0;
  cellno[ztime] = zcellno; 
  follno[ztime] = 0.0; 
  diffcellno[ztime] = 0.0; 
  pinterval = 0; 
  sointerval = 0;
  sositeno = psiteno * sopratio;
  
/* growth loop */
/* evaluate n(t),d(t),f(t) recursively over timeincrement day increments 
   in t until n(t)--> zero */
  /* repeat */
  while (time < maxtime) {
    time = time + timeincrement; 
    interval = time - ztime; 
    cellbirthprob = growthrate/time;
    weight = growthint*exp(growthrate*log((double)time));
    surfarea = surfareaconst*exp((2.0/3.0)*log(weight));
  
  if (cellno[time-timeincrement] <= 0) {
    /* out of founder cells */
	printf(" out  of founder cells \n");
        pfollno[time] = pfollno[time-timeincrement];
        sofollno[time] = sofollno[time-timeincrement];
        sdfollno[time] = sdfollno[time-timeincrement];
        diffcellno[time] = diffcellno[time-timeincrement];
        follno[time] = follno[time-timeincrement];
        cellno[time] = 0;   /* no undifferentiated cells remain */
    }
    else {
    /* calculate differentiated cell numbers & follicle types */
        if ((time >= pstarttime) && (pfollno[time-timeincrement]<psiteno))
            { 
            /* then primary period */ 
	      printf("primary period\n");
              pinterval = pinterval + timeincrement;
              ptime = time-pstarttime;
              pfollno[time] = follinitrate*surfarea*ptime;
              sofollno[time] = 0.0;
              sdfollno[time] = 0.0;
              diffcellno[time] = pfollno[time] * pavecellno;
              follno[time] = pfollno[time];
              pendtime = time;
            }
          else if (time<sostarttime) 
            {
	    /* then between periods if no overlap */
	      printf("between P and So periods\n");
              pfollno[time] = pfollno[time-timeincrement];
              sofollno[time] = 0.0;
              sdfollno[time] = 0.0;
              diffcellno[time] = pfollno[time] * pavecellno;
              follno[time] = pfollno[time];
            }
        if ((time>=sostarttime) && (sofollno[time-timeincrement]<sositeno))
            { 
            /* then so period */
	      printf("So period\n");
              sointerval = sointerval + timeincrement;
              sotime = time-sostarttime;
              pfollno[time] = pfollno[time-timeincrement]; 
              sofollno[time] = follinitrate*surfarea*sotime; 
              sdfollno[time] = 0.0;
              diffcellno[time] = pfollno[time] * pavecellno
                                  + sofollno[time] * soavecellno;
              follno[time] = pfollno[time] + sofollno[time];
              soendtime = time;
            }
       else if ((time>=sostarttime) && (sofollno[time-timeincrement]>=sositeno))
           {
           /* then  sd period */
	     printf("Sd period\n");
             sdtime = time - sostarttime - sointerval;
             pfollno[time] = pfollno[time-timeincrement]; 
             sofollno[time] = sofollno[time-timeincrement]; 
             sdfollno[time] = follinitrate*surfarea*sdtime;
             diffcellno[time] = pfollno[time] * pavecellno
                               + sofollno[time] * soavecellno
                               + sdfollno[time] * sdavecellno;
             follno[time] = pfollno[time]+sofollno[time]+sdfollno[time];
           }

    /* calculate undifferentiated cell numbers for start of next time interval
                                      ie for end of current time interval */
        cellno[time] = cellno[time-timeincrement]
                    + cellbirthprob * cellno[time-timeincrement] * timeincrement
                    - (diffcellno[time]-diffcellno[time-timeincrement]);
    } /* end else */
  
/* calculate densities */
  density(surfarea,&dens,&pdens,&sodens,&sddens,&sdens,&ratio,time,
	follno,pfollno,sofollno,sdfollno);
  
  
/* print result for this time */
    printf("%3d %9.0lf %8.0lf %9.0lf %9.0lf %11.0lf %11.0lf %8.1lf %8.1lf %8.0lf %8.5lf %6.0lf %5.0lf %6.0lf %6.0lf %6.0lf %5.1lf\n "
	    ,time,follno[time],pfollno[time] 
            ,sofollno[time],sdfollno[time]
            ,diffcellno[time],cellno[time]
            ,weight,surfarea,follinitrate*surfarea
            ,cellbirthprob,dens,pdens,sodens,sddens
            ,sdens,ratio);
  
  /* until time>=maxtime; */ 
  }
  
/* calculate adult densities */
  weight = adultweight;
  surfarea = surfareaconst*exp((2.0/3.0)*log(weight));
  density(surfarea,&dens,&pdens,&sodens,&sddens,&sdens,&ratio,time,
	follno,pfollno,sofollno,sdfollno);

  printf(" adult S+P density per cm sq = %8.0f\n",dens);
  printf(" adult P density per cm sq =  %8.0f\n",pdens);
  printf(" adult So density per cm sq = %8.0f\n",sodens);
  printf(" adult Sd density per cm sq = %8.0f\n",sddens);
  printf(" adult S density per cm sq = %8.0f\n",sdens);
  printf(" adult S/P ratio = %8.1f\n",ratio); 
  printf(" p interval = %d\n",pinterval);
  printf(" so interval = %d\n",sointerval);

  free(pfollno);
  free(sofollno);
  free(sdfollno);
  free(cellno);
  free(follno);
  free(diffcellno);

  exit(0);
}


  
void density(double surfarea,
		double *dens,
		double *pdens,
		double *sodens,
		double *sddens,
		double *sdens,
		double *ratio,
                int time,
                double* follno,
		double* pfollno,
		double* sofollno,
		double* sdfollno)
/* calculate densities - all variables global ????  */
{
  *dens = follno[time]/surfarea;
  *pdens = pfollno[time]/surfarea;
  *sodens = sofollno[time]/surfarea;
  *sddens = sdfollno[time]/surfarea;
  *sdens = (sofollno[time] + sdfollno[time])/surfarea;
  if (pfollno[time]>0) { 
      *ratio = (sofollno[time] + sdfollno[time])/pfollno[time];
    }
    else {
      *ratio = 0.0;
    }
  return;
}
  
