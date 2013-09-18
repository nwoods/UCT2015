#include "L1Trigger/UCT2015/interface/jetcorrections.h"


double jetcorrTAvg(double ptraw, double puLevel, int ieta) {

  double alphaLo[]={ 
    0.46665,
    0.44769,
    0.68489,
    1.28203,
    1.12002,
    0.83291,
    1.18307,
    1.24562,
    1.19178,
    1.24836,
    1.23230,
    1.24593,
    1.23045,
    1.17917,
    1.14891,
    1.12906,
    0.87794,
    1.15675,
    1.20266,
    0.57627,
    0.44556,
    0.39979};

    // First binned fits attempt
//     0.466657,
//     0.414719,
//     0.528533,
//     1.003114,
//     1.155088,
//     0.978547,
//     1.735807,
//     1.719914,
//     1.472535,
//     1.810460,
//     1.529383,
//     1.782944,
//     1.514645,
//     1.765001,
//     1.765963,
//     1.733089,
//     0.976448,
//     1.174523,
//     1.127315,
//     0.516848,
//     0.445563,
//     0.427807};

  double alphaHi[] = {
    1.54838,
    2.04785,
    1.89116,
    1.77373,
    1.32293,
    1.02199,
    1.07234,
    1.15976,
    1.14238,
    0.97014,
    1.10886,
    1.18708,
    1.09705,
    1.16156,
    0.99547,
    1.04254,
    1.07211,
    1.33072,
    1.53257,
    1.81841,
    1.98557,
    2.10749};


    // First binned fits attempt
//     1.418354,
//     1.917441,
//     1.873979,
//     1.821415,
//     1.290393,
//     1.001474,
//     1.035370,
//     1.132280,
//     1.174712,
//     1.128658,
//     1.157356,
//     1.120581,
//     1.185830,
//     1.128774,
//     1.126749,
//     1.045137,
//     1.042866,
//     1.175875,
//     1.725231,
//     1.875021,
//     1.985578,
//     2.471319};



    // 2nd 2-param attempt
//     0.861177,
//     1.08152, 
//     1.32221, 
//     1.45281, 
//     1.14661, 
//     0.957782,
//     0.958204,
//     1.02148, 
//     1.0377,  
//     1.02879, 
//     1.03208, 
//     1.02798, 
//     1.04281, 
//     1.04704, 
//     1.02597, 
//     0.977642,
//     0.997307,
//     1.15967, 
//     1.4297,  
//     1.32873, 
//     1.05633, 
//     0.856449};



  // 2 parameter first attempt
//     0.867711, 
//     1.02641,  
//     1.30385,  
//     1.52004,  
//     1.20444,  
//     0.980615, 
//     0.98932,  
//     1.05603,  
//     1.07091,  
//     1.06304,  
//     1.06518,  
//     1.06375,  
//     1.07798,  
//     1.08122,  
//     1.06115,  
//     1.00966,  
//     1.02007,  
//     1.2168,   
//     1.50508,  
//     1.30008,  
//     0.999099, 
//     0.864286};



    // redo with PU
//     0.895797,
//     1.08165, 
//     1.33042, 
//     1.53114, 
//     1.20438, 
//     0.98106, 
//     0.989534,
//     1.05707, 
//     1.07174, 
//     1.06398, 
//     1.06596, 
//     1.06432, 
//     1.07826, 
//     1.08201, 
//     1.06214, 
//     1.00979, 
//     1.02056, 
//     1.21793, 
//     1.51227, 
//     1.3214,  
//     1.05075, 
//     0.907389};


//     0.867711, 
//     1.02641,  
//     1.30385,  
//     1.52004,  
//     1.20444,  
//     0.980615, 
//     0.98932,  
//     1.05603,  
//     1.07091,  
//     1.06304,  
//     1.06518,  
//     1.06375,  
//     1.07798,  
//     1.08122,  
//     1.06115,  
//     1.00966,  
//     1.02007,  
//     1.2168 ,  
//     1.50508,  
//     1.30008,  
//     0.999099, 
//     0.864286};

//     0.917342, 1.08373, 1.33885, 1.53641, 
//     1.20536, 0.980639, 0.98932, 1.05603,
//     1.07091, 1.06307, 1.06522, 1.06377,
//     1.07799, 1.08121, 1.06115, 1.00966,
//     1.02009, 1.21765, 1.52313, 1.33939,
//     1.06182, 0.911627};

//   double beta[] = {
//     -0.289618, 
//     -0.797965, 
//     -0.807688, 
//     -0.915057, 
//     -1.50565,  
//     -1.94768,  
//     -1.89901,  
//     -2.10863,  
//     -2.08356,  
//     -1.83178,  
//     -1.7867,   
//     -1.87016,  
//     -1.82494,  
//     -1.88024,  
//     -1.9623,   
//     -1.7112,   
//     -1.83263,  
//     -1.63291,  
//     -0.888606, 
//     -0.878233, 
//     -0.760846, 
//     -0.89335};

  // 20 GeV cut
//     -0.678366,
//     -0.770852,
//     -0.873943,
//     -1.04581, 
//     -1.8838,  
//     -2.31705, 
//     -2.3501,  
//     -2.52118, 
//     -2.47518, 
//     -2.1402,  
//     -2.07349, 
//     -2.12401, 
//     -2.11872, 
//     -2.28946, 
//     -2.34456, 
//     -2.16784, 
//     -2.20118, 
//     -1.9539,  
//     -0.96018, 
//     -0.899818,
//     -0.745419,
//     -0.689394};

  double gammaLo[] = {
    10.6932,
    8.61901,
    5.05139,
    0.57333,
    10.6112,
    18.8885,
    11.9242,
    11.4369,
    9.79479,
    6.83388,
    5.48653,
    5.64547,
    7.61984,
    10.5568,
    12.8789,
    12.8988,
    18.4659,
    10.4896,
    1.51366,
    6.36156,
    8.38699,
    11.4079};


    // First binned fits attempt
//     10.693263,
//     9.0602034,
//     7.1727362,
//     3.9100388,
//     10.128035,
//     16.914562,
//     5.2816157,
//     5.5926851,
//     5.9881098,
//     -0.104920,
//     1.6319342,
//     -0.893378,
//     3.8576581,
//     3.0696536,
//     5.0419853,
//     5.3755754,
//     17.269524,
//     10.251991,
//     2.4361113,
//     7.1555128,
//     8.3869941,
//     11.109189};

  double gammaHi[] = {
    -9.89417,
    -35.6980,
    -25.8411,
    -9.70798,
    -5.93388,
    11.92622,
    11.91462,
    9.551129,
    10.44106,
    19.98525,
    10.58970,
    4.820442,
    12.68225,
    9.574046,
    21.06262,
    14.28858,
    10.89563,
    -3.71242,
    1.488775,
    -22.6312,
    -34.4997,
    -22.8437};


    // First binned fits attempt
//     -6.822381,
//     -29.73164,
//     -24.73324,
//     -11.50145,
//     -2.964365,
//     12.897384,
//     14.233442,
//     11.593649,
//     7.8385627,
//     9.2009976,
//     5.9115467,
//     8.3911234,
//     6.5780184,
//     10.670809,
//     11.922624,
//     14.180009,
//     12.582349,
//     4.9883622,
//     -7.060265,
//     -25.01945,
//     -34.49974,
//     -31.72146};



    // Second attempt at a 2-param fit
//       16.2379,
//       8.72267,
//       5.02399,
//       9.1284, 
//       20.5158,
//       19.9677,
//       22.9062,
//       21.8307,
//       20.0085,
//       19.036, 
//       17.62,  
//       18.09,  
//       18.9415,
//       20.0258,
//       21.9646,
//       22.4728,
//       19.5259,
//       20.4366,
//       9.44458,
//       4.33289,
//       9.33362,
//       16.8903};

    // First attempt at 2-param fit
//     11.2146,
//     5.67163,
//     1.89038,
//     4.07677,
//     16.8013,
//     18.2999,
//     20.56,	 
//     19.362, 
//     17.3111,
//     16.1556,
//     14.7054,
//     15.063, 
//     16.0927,
//     17.3563,
//     19.4758,
//     20.1121,
//     17.9698,
//     16.8004,
//     4.23236,
//     1.72167,
//     6.12111,
//     11.1815};


  // 30 GeV cut
//     17.1074,
//     12.7079,
//     9.72478,
//     13.3661,
//     23.7897,
//     22.277,	
//     25.2262,
//     24.8477,
//     23.4421,
//     22.6543,
//     21.5988,
//     22.2668,
//     22.4577,
//     23.1484,
//     24.785,	
//     24.5578,
//     21.6208,
//     23.797,	
//     13.6186,
//     9.63565,
//     13.1498,
//     19.186};


  // 20 GeV cut
//     13.5094,
//     9.88804,
//     7.36729,
//     9.08225,
//     21.0233,
//     21.0789,
//     23.4619,
//     23.0103,
//     21.4364,
//     20.431, 
//     19.3789,
//     19.8612,
//     20.2114,
//     21.1979,
//     22.8835,
//     22.7818,
//     20.5152,
//     20.9407,
//     8.8169, 
//     7.52497,
//     10.272, 
//     13.3236};

//     11.2146,
//     5.67163,
//     1.89038,
//     4.07677,
//     16.8013,
//     18.2999,
//     20.56,  
//     19.362, 
//     17.3111,
//     16.1556,
//     14.7054,
//     15.063, 
//     16.0927,
//     17.3563,
//     19.4758,
//     20.1121,
//     17.9698,
//     16.8004,
//     4.23236,
//     1.72167,
//     6.12111,
//     11.1815};
  

//     10.0278, 3.46754, 0.172832, 3.23472, 
//     16.7919, 18.3008, 20.56, 19.362, 17.3111,
//     16.1558, 14.7058, 15.0636, 16.0928, 
//     17.3564, 19.4758, 20.1121, 17.9707, 
//     16.6958, 3.22106, -0.210373, 3.78894, 
//     9.88917};

  double cutoff[] = {20.,25.,25.,20.,45.,
		     25.,25.,25.,30.,25.,
		     30.,25.,30.,25.,25.,
		     25.,25.,45.,30.,25.,
		     30.,23};

  double pt;

  if(ptraw < cutoff[ieta])
    {
      pt =  alphaLo[ieta] * ptraw /*+ beta[ieta] * puLevel*/ 
	+ gammaLo[ieta];
    }
  else
    {
      pt =  alphaHi[ieta] * ptraw /*+ beta[ieta] * puLevel*/ 
	+ gammaHi[ieta];
    }

  return pt;
}

double jetcorr(double ptraw, int ieta, unsigned int pu) {

  double calib[]={ // no cuts at all
    0.892188, -0.651711, 10.2378, 4.58121,
    1.02586, -1.33642, 4.81479, 9.36397,
    1.27058, -1.50567, 2.07494, 10.4648,
    1.50522, -1.31665, 4.08716, 8.85142,
    1.26861, -0.835476, 16.0302,5.40097,
    1.21747, -0.390929, 21.335, 2.47538,
    1.24958, -0.222715, 21.7479,1.56268,
    1.23016, -0.280122, 22.383,1.79817,
    1.20706, -0.294144, 20.5458,1.92008,
    1.16759, -0.268146, 19.5496,1.74353,
    1.18027, -0.343445, 17.7221,2.23023,
    1.18262, -0.330986, 18.0593,2.13182,
    1.20777, -0.34207, 19.1141, 2.22131,
    1.23128, -0.327868, 20.1695,2.12529,
    1.26521, -0.301663, 21.5901,1.9261,
    1.22042, -0.297534, 22.2525,1.93023,
    1.21491, -0.296532, 22.0371,1.91047,
    1.28441, -0.810513, 15.964,5.24753,
    1.49075, -1.28715, 4.51183,8.75857,
    1.30826, -1.49382, 1.10413,10.3965,
    0.991312, -1.25799, 5.38376,8.87343,
    0.938214, -0.710223, 9.25766,5.20865,};

  double alpha = calib[3*ieta + 0];
  double beta  = calib[3*ieta + 1];
  double gamma = calib[3*ieta + 2];
  double delta = calib[4*ieta + 3];       

  double pt =  alpha * ptraw + beta * pu + gamma + delta;

  return pt;
}


double jetcorrUIC(double ptraw, int ieta, unsigned int pu) {


  double calib[]={ // no cuts at all
    0.892188, -0.651711, 10.2378, 4.58121,
    1.02586, -1.33642, 4.81479, 9.36397,
    1.27058, -1.50567, 2.07494, 10.4648,
    1.50522, -1.31665, 4.08716, 8.85142,
    1.26861, -0.835476, 16.0302,5.40097,
    1.21747, -0.390929, 21.335, 2.47538,
    1.24958, -0.222715, 21.7479,1.56268,
    1.23016, -0.280122, 22.383,1.79817,
    1.20706, -0.294144, 20.5458,1.92008,
    1.16759, -0.268146, 19.5496,1.74353,
    1.18027, -0.343445, 17.7221,2.23023,
    1.18262, -0.330986, 18.0593,2.13182,
    1.20777, -0.34207, 19.1141, 2.22131,
    1.23128, -0.327868, 20.1695,2.12529,
    1.26521, -0.301663, 21.5901,1.9261,
    1.22042, -0.297534, 22.2525,1.93023,
    1.21491, -0.296532, 22.0371,1.91047,
    1.28441, -0.810513, 15.964,5.24753,
    1.49075, -1.28715, 4.51183,8.75857,
    1.30826, -1.49382, 1.10413,10.3965,
    0.991312, -1.25799, 5.38376,8.87343,
    0.938214, -0.710223, 9.25766,5.20865,};

  double alpha = calib[4*ieta + 0];
  double beta  = calib[4*ieta + 1];
  double gamma = calib[4*ieta + 2];
  double delta = calib[4*ieta + 3];       

  double pt =  alpha * ptraw + beta * pu + gamma + delta;
  return pt;
}
