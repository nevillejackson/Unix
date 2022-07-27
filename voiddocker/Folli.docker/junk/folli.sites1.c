program mooresfolli (input,output); 
(*deterministic model for founder cell population growth
  and follicle initiation*) 
const 
  ztime=60; 
  maxtime=150;
  timeincrement=1;
  surfareaconst=9.0;
var 
  time,interval,pstarttime,pinterval,pendtime,sostarttime,soendtime 
  ,sointerval,ptime,sotime,sdtime
   : integer; 
  weight,growthint,surfarea, growthrate,cellbirthprob,follinitrate
  ,pavecellno,soavecellno,sdavecellno 
  ,zcellno,ratio,pdens,dens,siteno
   : real;
  cellno,follno,diffcellno,pfollno,sofollno,sdfollno
   : array [ztime..maxtime] of real;
begin 
  
(*enter parameters*)
  write(' number of sites ');
  read(siteno); 
  write(siteno:5:0);
  writeln;
  write(' growthrate - slope of log_wt/log_age line '); 
  read(growthrate);
  write(growthrate:8:5); 
  writeln;
  write(' growth intercept - of log_wt/log_age line ');
  read(growthint);
  writeln(growthint:12:10);
  write(' follicle initiation rate - increase per timeincrement ');
  read(follinitrate); 
  write(follinitrate:8:4);
  writeln;
  write(' times of start & end of primary follicle initiation period '); 
  read(pstarttime,pendtime);
  write(pstarttime:8:1,pendtime:8:1); 
  writeln;
  write(' time of start of secondary original follicle initiation period '); 
  read(sostarttime);
  writeln(sostarttime:8:1); 
  write(' number of founder cells at time zero '); 
  read(zcellno);
  write(zcellno:8:1); 
  writeln;
  write(' average number of cells per p,so,sd follicle'); 
  read(pavecellno,soavecellno,sdavecellno); 
  write(pavecellno:8:3,soavecellno:8:3,sdavecellno:8:3);
  writeln;
  
(*evaluate n(t),d(t),f(t) recursively over timeincrement day increments 
  in t until n(t)--> zero *)
  write(' time      foll     pfoll    sofoll    sdfoll    diffoundcell   foundcell'); 
  writeln('  weight surfarea follirate');
  time := ztime;
  cellno[ztime] := zcellno; 
  follno[ztime] := 0.0; 
  diffcellno[ztime] := 0.0; 
  pinterval := pendtime-pstarttime+1; 
  sointerval := 0;
  
  repeat
    time := time + timeincrement; 
    interval := time - ztime; 
    cellbirthprob := growthrate/time;
    weight := growthint*exp(growthrate*ln(time));
    surfarea := surfareaconst*exp((2.0/3.0)*ln(weight));
  
(*calculate differentiated cell numbers & follicle types*)
    if (time >= pstarttime) and (time <= pendtime)
      then (*primary period*) 
        begin 
          ptime := time-pstarttime;
          pfollno[time] := follinitrate*surfarea*ptime;
          sofollno[time] := 0.0;
          sdfollno[time] := 0.0;
          diffcellno[time] := pfollno[time] * pavecellno;
          follno[time] := pfollno[time]
        end
    else if (time>pendtime) and (time<sostarttime)
      then (* between p and so periods *)
        begin
          pfollno[time] := pfollno[time-timeincrement];
          sofollno[time] := 0.0;
          sdfollno[time] := 0.0;
          diffcellno[time] := pfollno[time]*pavecellno;
          follno[time] := pfollno[time]
        end
    else if (time>=sostarttime) and (follno[time-timeincrement]<siteno)
      then (*so period*)
        begin 
          sointerval := sointerval + timeincrement;
          sotime := time-sostarttime;
          pfollno[time] := pfollno[time-timeincrement]; 
          sofollno[time] := follinitrate*surfarea*sotime; 
          sdfollno[time] := 0.0;
          diffcellno[time] := pfollno[time] * pavecellno
                              + sofollno[time] * soavecellno;
          follno[time] := pfollno[time] + sofollno[time]
        end
   else if follno[time-timeincrement]>=siteno
     then (* sd period *)
        begin
          sdtime := time - sostarttime - sointerval;
          pfollno[time] := pfollno[time-timeincrement]; 
          sofollno[time] := sofollno[time-timeincrement]; 
          sdfollno[time] := follinitrate*surfarea*sdtime;
          diffcellno[time] := pfollno[time] * pavecellno
                            + sofollno[time] * soavecellno
                            + sdfollno[time] * sdavecellno;
          follno[time] := pfollno[time]+sofollno[time]+sdfollno[time]
        end;
  
(*calculate undifferentiated cell numbers *)
    cellno[time] := cellno[time-timeincrement]
                    + cellbirthprob * cellno[time-timeincrement] * timeincrement
                    - (diffcellno[time]-diffcellno[time-timeincrement]);
  
(*print result for this time*)
    writeln(' ',time:4,follno[time]:10:1,pfollno[time]:10:1 
            ,sofollno[time]:10:1,sdfollno[time]:10:1
            ,diffcellno[time]:16:1,cellno[time]:12:1
            ,weight:8:1,surfarea:9:1,follinitrate*surfarea:10:1
            ,ptime:4,sotime:4,sdtime:4,cellbirthprob:8:5);
  
  until (cellno[time]<=0)or(time>=maxtime); 
  ratio := (sofollno[time] + sdfollno[time])/pfollno[time]; 
  pdens := pfollno[time]/surfarea;
  dens := follno[time]/surfarea;
  writeln(' birth P density per cm sq = ',pdens:8:1);
  writeln(' birth S+P density per cm sq = ',dens:8:1);
  writeln(' birth S/P ratio = ',ratio:8:1); 
  writeln(' p interval = ',pinterval);
  writeln(' so interval = ',sointerval);
end.
