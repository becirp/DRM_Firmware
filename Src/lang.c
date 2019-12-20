#include "lang.h"

PRINTER_TEXT_STRUCT activetext;

#if 0
PRINTER_TEXT_STRUCT printer_texts[1] = {
//__eeprom PRINTER_TEXT_STRUCT printer_texts[1] = {
    0,                                          //1
    {"Out of range"},                           //2
    {"TEST REPORT"},                            //3
    {"Circuit Breaker Analyzer & Timer"},       //4
    {"Date"},                                   //5
    {"Time"},                                   //6
    {"BREAKER DATA"},                           //7
    {"Station"},                                //8
    {"Breaker ID"},                             //9
    {"Department"},                             //10
    {"Serial Number"},                          //11
    {"Manufacturer"},                           //12
    {"Breaker Type"},                           //13
    {"TEST DATA"},                              //14
    {"Type of Test"},                           //15
    {"Company Name"},                           //16
    {"Operator"},                               //17
    {"COMMENTS"},                               //18
    ////////////////////////////////  
    {"GENERAL TEST CONDITIONS"},                //19
    {"Sequence"},                               //20
    {"Measuring time"},                         //21
    {"Close pulse"},                            //22
    {"CO Delay"},                               //23
    {"First Contact Touch"},                    //24
    {"Open pulse"},                             //25
    {"OC Delay"},                               //26
    {"OCO Delay"},                              //27
    {"Trigger"},                                //28
    {"External"},                               //29
    {"Internal"},                               //30
    {"Transducer"},                             //31
    {"Digital"},                                //32
    {"Linear"},                                 //33
    {"Break Units per phase"},                  //34
    {"TIMING RESULTS"},                         //35
    ////////////////////////////////
    {"RESISTANCE MEASUREMENT"},                 //36
    ///////////////////////////////
    {"Opening time:"},                          //37
    {"Closing time:"},                          //38
    {"Phase Opening time:"},                    //39
    {"Phase Closing time:"},                    //40
    {"Phase Synchronism"},                      //41
    {"Breaker Opening time"},                  //42
    {"Breaker Closing time"},                  //43
    {"AUX Opening time:"},                      //44
    {"AUX Closing time:"},                      //45
    {"O Synchronization:"},                     //46
    {"C Synchronization:"},                     //47
    {"Close-Open time:"},                       //48
    {"Phase C-O time:"},                        //49
    {"Breaker C-O time:"},                      //50
    {"Reclosing time:"},                        //51
    {"Phase reclosing time"},                   //52
    {"Breaker Reclos. time"},                   //53
    {"Open-Close time:"},                       //54
    {"Phase O-C time:"},                        //55
    {"Breaker O-C time:"},                      //56
    {"O1 Synchronization:"},                    //57
    {"O2 Synchronization:"},                    //58
    {"Preinsertion R time:"},                   //59
    {"Resistor ON time:"},                      //60
    {"Resistance value:"},                      //61
    {"Breaker R Synchron.:"},                   //62
    {"Phase R ON time:"},                       //63
    /////////////////////////
    {"Maximum Current"},                        //64
    {"Closing Coil"},                           //65
    {"Trip Coil"},                              //66
    ////////////////////////
    {"Analog"},                                 //67
    {"Range"},                                  //68
    ////////////////////////
    {"Motion Results"},                         //69
    {"Contact Wipe:"},                          //70
    {"Motion results are"},                     //71
    {"available only for"},                     //72
    {"Open and Close test!"},                   //73 
    {"CURRENT RESULTS"},                        //74
    {"ANALOG CH. RESULTS"},                     //75
    {"TRANSDUCER RESULTS"},                     //76
    {"PREINSERTION RESISTOR"},                  //77
    {"ST"},                                     //78
    {"OT"},                                     //79
    {"RB"},                                     //80
    {"GRAPH"},                                  //81 
    {"Horizontal Scale:"},                      //82
    {"Vertical Scale:"},                        //83
    {"Closing Coil Current:"},                  //84 
    {"Current"},                                //85 
    {"MAIN CONTACTS"},                          //86
    {"AUX CHANNELS   CURRENTS"},                //87 
    {"CURRENTS          AUX CHANNELS"}          //88
};
#endif

#if 1
//EVENTUALNO UCITATI SA SD CARD PO PALJENJU APARATA ZBOG USTEDE MEMORIJE:
PRINTER_TEXT_STRUCT printer_texts[2] =  {
//English:
{
    0,                                          //1
    {"Out of range"},                           //2
    {"TEST REPORT"},                            //3
    {"Circuit Breaker Analyzer & Timer"},       //4
    {"Date"},                                   //5
    {"Time"},                                   //6
    {"BREAKER DATA"},                           //7
    {"Station"},                                //8
    {"Breaker ID"},                             //9
    {"Department"},                             //10
    {"Serial Number"},                          //11
    {"Manufacturer"},                           //12
    {"Breaker Type"},                           //13
    {"TEST DATA"},                              //14
    {"Type of Test"},                           //15
    {"Company Name"},                           //16
    {"Operator"},                               //17
    {"COMMENTS"},                               //18
    ////////////////////////////////  
    {"GENERAL TEST CONDITIONS"},                //19
    {"Sequence"},                               //20
    {"Measuring time"},                         //21
    {"Close pulse"},                            //22
    {"CO Delay"},                               //23
    {"First Contact Touch"},                    //24
    {"Open pulse"},                             //25
    {"OC Delay"},                               //26
    {"OCO Delay"},                              //27
    {"Trigger"},                                //28
    {"External"},                               //29
    {"Internal"},                               //30
    {"Transducer"},                             //31
    {"Digital"},                                //32
    {"Linear"},                                 //33
    {"Break Units per phase"},                  //34
    {"TIMING RESULTS"},                         //35
    ////////////////////////////////
    {"RESISTANCE MEASUREMENT"},                 //36
    ///////////////////////////////
    {"Opening time:"},                          //37
    {"Closing time:"},                          //38
    {"Phase Opening time:"},                    //39
    {"Phase Closing time:"},                    //40
    {"Phase Synchronism"},                      //41
    {"Breaker Opening time"},                  //42
    {"Breaker Closing time"},                  //43
    {"AUX Opening time:"},                      //44
    {"AUX Closing time:"},                      //45
    {"O Synchronization:"},                     //46
    {"C Synchronization:"},                     //47
    {"Close-Open time:"},                       //48
    {"Phase C-O time:"},                        //49
    {"Breaker C-O time:"},                      //50
    {"Reclosing time:"},                        //51
    {"Phase reclosing time"},                   //52
    {"Breaker Reclos. time"},                   //53
    {"Open-Close time:"},                       //54
    {"Phase O-C time:"},                        //55
    {"Breaker O-C time:"},                      //56
    {"O1 Synchronization:"},                    //57
    {"O2 Synchronization:"},                    //58
    {"Preinsertion R time:"},                   //59
    {"Resistor ON time:"},                      //60
    {"Resistance value:"},                      //61
    {"Breaker R Synchron.:"},                   //62
    {"Phase R ON time:"},                       //63
    /////////////////////////
    {"Maximum Current"},                        //64
    {"Closing Coil"},                           //65
    {"Trip Coil"},                              //66
    ////////////////////////
    {"Analog"},                                 //67
    {"Range"},                                  //68
    ////////////////////////
    {"Motion Results"},                         //69
    {"Contact Wipe:"},                          //70
    {"Motion results are"},                     //71
    {"available only for"},                     //72
    {"Open and Close test!"},                   //73 
    {"CURRENT RESULTS"},                        //74
    {"ANALOG CH. RESULTS"},                     //75
    {"TRANSDUCER RESULTS"},                     //76
    {"PREINSERTION RESISTOR"},                  //77
    {"ST"},                                     //78
    {"OT"},                                     //79
    {"RB"},                                     //80
    {"GRAPH"},                                  //81 
    {"Horizontal Scale:"},                      //82
    {"Vertical Scale:"},                        //83
    {"Closing Coil Current:"},                  //84 
    {"Current"},                                //85 
    {"MAIN CONTACTS"},                          //86
    {"AUX CHANNELS   CURRENTS"},                //87 
    {"CURRENTS          AUX CHANNELS"}          //88
},    
//French:
{
    1,                                          //1
    {"Hors gamme"},                             //2
    {"RAPPORT D'ESSAI"},                        //3
    {"    Analyseur de Disjoncteur"},           //4
    {"Date"},                                   //5
    {"Heure"},                                  //6
    {"Donnees sur disj"},                       //7
    {"Nom de station"},                         //8
    {"Disjoncteur ID"},                         //9
    {"Departement"},                            //10
    {"Numero de serie"},                        //11
    {"Fabricant"},                              //12
    {"Type de disjoncteur"},                    //13
    {"Donnees sur essai"},                      //14
    {"Type d'essai:"},                          //15
    {"Nom de societe:"},                        //16
    {"Operateur"},                              //17
    {"COMMENTAIRES"},                           //18
    //////////////////////////////// 
    {"CONDITIONS GENERALES D'ESSAI"},           //19
    {"Sequence"},                               //20
    {"Temps de mesure"},                        //21
    {"Impulsion de fermeture"},                 //22
    {"Delai FO"},                               //23
    {"Premier"},                                //24
    {"Impulsion d'ouvert."},                    //25
    {"Delai OF"},                               //26
    {"Delai OFO"},                              //27
    {"Declencheur"},                            //28
    {"Externe"},                                //29
    {"Interne"},                                //30
    {"Capteur"},                                //31
    {"Numerique"},                              //32
    {"Lineaire"},                               //33
    {"Nombre d'elements de coup par phase"},    //34
    {"RESULTATS DE TEMPS"},                     //35
    //////////////////////////////// 
    {"MESURE DE RESISTANCE"},                   //36
    ///////////////////////////////
    {"Temps d'ouverture:"},                     //37
    {"Temps de fermeture:"},                    //38
    {"Phase temp d'ouvert:"},                   //39
    {"Phase temp de ferm:"},                    //40
    {"Phase Synchronisme:"},                    //41
    {"Temp d'ouvert Disj:"},                    //42
    {"Temp de ferm Disj:"},                     //43
    {"Temp d'ouvert AUX:"},                     //44
    {"Temp de ferm AUX:"},                      //45
    {"Synchronisation O:"},                     //46
    {"Synchronisation F:"},                     //47
    {"Temp Ferm-Ouvert:"},                      //48
    {"Phase temps F-O:"},                       //49
    {"Disjonct temp F-O:"},                     //50
    {"Temps de Referm:"},                       //51
    {"Phas temp de Referm"},                    //52
    {"Disj temp de Referm"},                    //53
    {"Temp Ouvert-Fermet:"},                    //54
    {"Phase temps O-F:"},                       //55
    {"Disj temp O-F:"},                         //56
    {"O1 Synchronisation:"},                    //57
    {"O2 Synchronisation:"},                    //58
    {"Temp R pre-insert:"},                     //59
    {"Temp d'activ de Res:"},                   //60
    {"Valeur de Res:"},                         //61
    {"Disj R Synchron.:"},                      //62
    {"Phas temp d'activ R:"},                   //63
    ///////////////////////////////
    {"Courant maximal"},                        //64
    {"Bobine de fermeture"},                    //65
    {"Bobine d'ouverture"},                     //66
    ///////////////////////////////
    {"Analogique"},                             //67
    {"Gamme"},                                  //68
    ///////////////////////////////
    {"Res de mouvement"},                       //69
    {"Glissement contact:"},                    //70
    {"Res de mouvement"},                       //71
    {"disponibles pour"},                       //72
    {"les essais O et F!"},                     //73
    {"RESULTATS DE COURANT"},                   //74
    {"CH. ANALOGIQUE RESULTATS"},               //75
    {"RESULTATS DE CAPTEUR"},                   //76   
    {"RESISTANCE DE PRE-INSERTION"},            //77
    {"CO"},                                     //78
    {"SC"},                                     //79
    {"RB"},                                     //80
    {"GRAPHIQUE"},                              //81 
    {"Echelle horizontale:"},                   //82
    {"Echelle verticale:"},                     //83
    {"Courant de bobine de fermeture:"},        //84
    {"Courant"},                                //85
    {"CONTACTS PRINCIPAUX"},                    //86
    {"CANAL AUX   COURANTS"},                   //87
    {"COURANTS          CANNAL AUX"}            //88
},                      
};
#endif
