	//  DHT22 
//  Asynch type
//  ESP32 DEV KIT V1
#include <WiFi.h>

//SOFTAPI
#include <WiFiClient.h>
#include <WiFiAP.h>

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>


//Variables
#define DHTPIN 12
#define DHTTYPE DHT22 //redeclare variable
DHT dht(DHTPIN, DHTTYPE);
String arrayA[5]={"--","---","----","-----","------"};
String arrayB[5]={"--","---","----","-----","------"};
String arrayC[5]={"--","---","----","-----","------"};
float Temperature;
float Humidity;
int soilSensorValue = 0;
int thresholdUp = 1900;
int thresholdDown = 1800;
int threshold = 650;
int waterpump = 26;
int sensorValue;
int limit = 300;
//Soft API
const char* ssid = "Keanu_Temperature_WiFi";
const char* password = "keanupass123";

//start the server
AsyncWebServer server(80);


// --------------------------- GETTING TEMPERATURE FROM DHT22 ----------------------------- //
 String BTemp(){
   arrayA[1] = arrayA[0];
   return String(arrayA[1]);
  };
 String CTemp(){
   arrayA[2] = arrayA[1];
   return String(arrayA[2]);
  };
 String DTemp(){
   arrayA[3] = arrayA[2];
   return String(arrayA[3]);
  };
 String ETemp(){
   arrayA[4] = arrayA[3];
   return String(arrayA[4]);
  };

 String ATemp(){
    float t = dht.readTemperature();
    if(isnan(t)){
      Serial.println("---");
      String Error = "---";
      return String(Error);
    }else{
      arrayA[0] = String(t);
      String temptext = "Temperature: "+ arrayA[0];
      Serial.println(temptext);
      return String(arrayA[0]);
    }
  };


// --------------------------- SOIL SENSOR VALUE  ---------------------------------------//

 String BSoil(){
   arrayC[1] = arrayC[0];
   return String(arrayC[1]);
  };
 String CSoil(){
   arrayC[2] = arrayC[1];
   return String(arrayC[2]);
  };
 String DSoil(){
   arrayC[3] = arrayC[2];
   return String(arrayC[3]);
  };
 String ESoil(){
   arrayC[4] = arrayC[3];
   return String(arrayC[4]);
  };

 String ASoil(){
    soilSensorValue = analogRead(34);
    soilSensorValue = map(soilSensorValue,0,1023,0,500); //map
    Serial.print("Soil Value: ");
    Serial.println(soilSensorValue);
    if(isnan(soilSensorValue)){
      Serial.println("---");
      String Error = "---";
      return String(Error);
    }else{
      arrayC[0] = String(soilSensorValue);
       if(soilSensorValue > threshold){
            digitalWrite(waterpump,LOW);
        }else{
            digitalWrite(waterpump,HIGH);
        };
      return String(arrayC[0]);
    }
    
 }
// --------------------------- GETTING HUMIDITY FROM DHT22 ----------------------------- //
String BHum(){
   arrayB[1] = arrayB[0];
   return String(arrayB[1]);
  };
 String CHum(){
   arrayB[2] = arrayB[1];
   return String(arrayB[2]);
  };
 String DHum(){
   arrayB[3] = arrayB[2];
   return String(arrayB[3]);
  };
 String EHum(){
   arrayB[4] = arrayB[3];
   return String(arrayB[4]);
  };

 String AHum(){
    float h = dht.readHumidity();
    if(isnan(h)){
      Serial.println("---");
      String Error = "---";
      return String(Error);
    }else{
      arrayB[0] = String(h);
      String text = "Humidity: "+ arrayB[0];
      Serial.println(text);
      return String(arrayB[0]);
    }
  };

// ------------------------- GETTING DATA FROM DHT22(2) -------------------------- //

//---------------- HTML CODE --------------------//
  const char index_html[] PROGMEM = R"rawliteral(
<html lang="en">
<head>
    <meta charset="UTF-8">
    <!-- <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0"> -->
    <title>Automated Plant Watering System</title>

    <!-- STYLES -->
    <style>
        .dash-grid {
            display: grid;
            grid-template-columns: 57vw;            
        }

        .dash-grid2 {
            display: grid;
            grid-template-rows: 5vw 70vh;
            width: 33vw;
        }

        .temp-grid {
            display: grid;
            grid-template-rows: 5vw 45vh;
            width: 20vw;
        }

        body {
            padding: 3rem;
        }

        .row {
            width: 100%;
            display: flex;
        }

        .column1{
            margin-right: 3vw;
            display: flex;
            flex-direction: column;
        }

        .flex {
            display: flex; 
            justify-content: center; 
            align-items: center;
        }

        .column1 .flex {
            flex-direction: column;
            align-items: flex-start;
        }

        .temp-grid .box {
            align-items: center;
        }
        
    </style>
</head>
<body>
    <div class="row">
    <div class="column1">
    <!-- WELCOME TO DASHBOARD -->
    <div class="dash-grid">
        <div class="box flex" style="background-color: #F3F5FE; border-radius: 20px; padding: 25px; padding-left: 60px;">
        <p style="color: #00575C; font-size: 35px; font-weight: bolder; font-family:'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;margin-bottom: 10px;">Welcome to your Dashboard</p>
        <p style="color: #00575C; font-size: 25px; font-family:'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; margin-top: 0px; ">Automated Plant Watering System</p>
        </div>
    </div>

    <!-- TEMP AND HUMIDITY -->
    <div class="row" style="margin-top: 3rem; justify-content: center;">

        <div class="temp-col">
            <div class="temp-grid">
                <div class="box flex" style="background-color: #58BA89; border-radius: 20px 20px 0px 0px; padding: 5px;"><p style="color: white; font-family:'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; font-size: 25px; font-weight: bolder; text-align: center;">Temperature</p></div>
        
                <!-- BODY DIV -->
                <div class="box flex" style="background-color: white; border-radius: 0px 0px 20px 20px; padding: 5px; box-shadow: 0px 4px 5px #C1C1C1; justify-content: flex-start;">
                
                    <!-- TEMPERATURE READINGS -->
                    <p id="Atemp_ID"; style="color: #00575C; font-family:'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; font-size: 25px; font-weight: bolder; text-align: left; margin-bottom: 10px;">----</p>
                    <p id="Btemp_ID"; style="color: #00575C; font-family:'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; font-size: 25px; font-weight: bolder; text-align: left; margin-bottom: 10px;">----</p>
                    <p id="Ctemp_ID"; style="color: #00575C; font-family:'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; font-size: 25px; font-weight: bolder; text-align: left; margin-bottom: 10px;">----</p>
                    <p id="Dtemp_ID"; style="color: #00575C; font-family:'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; font-size: 25px; font-weight: bolder; text-align: left; margin-bottom: 10px;">----</p>
                    <p id="Etemp_ID"; style="color: #00575C; font-family:'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; font-size: 25px; font-weight: bolder; text-align: left; margin-bottom: 10px;">----</p>
                
                </div>
               
            </div>
        
        </div>

        <div class="humid-col" style="margin-left: 7rem;">
            <div class="temp-grid">
                <div class="box flex" style="background-color: #00575C; border-radius: 20px 20px 0px 0px; padding: 5px;"><p style="color: white; font-family:'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; font-size: 25px; font-weight: bolder; text-align: center;">Humidity</p></div>
        
                <!-- BODY DIV -->
                <div class="box flex" style="background-color: white; border-radius: 0px 0px 20px 20px; padding: 5px; box-shadow: 0px 4px 5px #C1C1C1; justify-content: flex-start;">
                
                    <!-- HUMIDITY READINGS -->
                    <p id="Ahum_ID"; style="color: #00575C; font-family:'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; font-size: 25px; font-weight: bolder; text-align: left; margin-bottom: 10px;">----</p>
                    <p id="Bhum_ID"; style="color: #00575C; font-family:'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; font-size: 25px; font-weight: bolder; text-align: left; margin-bottom: 10px;">----</p>
                    <p id="Chum_ID"; style="color: #00575C; font-family:'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; font-size: 25px; font-weight: bolder; text-align: left; margin-bottom: 10px;">----</p>
                    <p id="Dhum_ID"; style="color: #00575C; font-family:'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; font-size: 25px; font-weight: bolder; text-align: left; margin-bottom: 10px;">----</p>
                    <p id="Ehum_ID"; style="color: #00575C; font-family:'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; font-size: 25px; font-weight: bolder; text-align: left; margin-bottom: 10px;">----</p>
                
                </div>
               
            </div>
        
        </div>
    </div>

</div>

    <div class="column2">
    <!-- WELCOME TO DASHBOARD -->
    <div class="dash-grid2">
        <div class="box flex" style="background-color: #58BA89; border-radius: 20px 20px 0px 0px; padding: 5px;"><p style="color: white; font-family:'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; font-size: 25px; font-weight: bolder;text-align: center;">Soil Moisture Value</p></div>

        <!-- BODY DIV -->
        <div class="box flex" style="background-color: white; border-radius: 0px 0px 20px 20px; padding: 5px; box-shadow: 0px 4px 5px #C1C1C1; flex-direction: column; justify-content: flex-start;">

            <p id="ASoil_ID"; style="color: #00575C; font-family:'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; font-size: 25px; font-weight: bolder; text-align: left; margin-bottom: 10px;">---</p>
            <p id="BSoil_ID"; style="color: #00575C; font-family:'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; font-size: 25px; font-weight: bolder; text-align: left; margin-bottom: 10px;">---</p>
            <p id="CSoil_ID"; style="color: #00575C; font-family:'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; font-size: 25px; font-weight: bolder; text-align: left; margin-bottom: 10px;">---</p>
            <p id="DSoil_ID"; style="color: #00575C; font-family:'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; font-size: 25px; font-weight: bolder; text-align: left; margin-bottom: 10px;">---</p>
            <p id="ESoil_ID"; style="color: #00575C; font-family:'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; font-size: 25px; font-weight: bolder; text-align: left; margin-bottom: 10px;">---</p>        
        </div>
       
    </div>

</div>
</div>

    
</body>
<!-- -----------------FRONT END----------------- -->


<!-- -----------------BACK END/SCRIPT----------------- -->
<script>
  <!-- -----REFRESHING HUMIDITY W/O Refreshing Webpage------ -->
  setInterval(function(){
    var Htp_Req = new XMLHttpRequest();                                   <!-- requesting data -->
     Htp_Req.onreadystatechange = function(){                             <!-- preparing data  -->
      if(this.readyState == 4 && this.status == 200){                     <!-- checking if data is ready -->
        document.getElementById("Ehum_ID").innerHTML = this.responseText; <!-- Finding html ID what to replace -->
      }
    };
     Htp_Req.open("GET","/Ehum_ID",true);                                 <!-- Getting the Data prepared by the hardware(see void setup)-->
     Htp_Req.send();                                                      <!-- send data -->
  }, 5000);

  setInterval(function(){
    var Htp_Req = new XMLHttpRequest();                                   <!-- requesting data -->
     Htp_Req.onreadystatechange = function(){                             <!-- preparing data  -->
      if(this.readyState == 4 && this.status == 200){                     <!-- checking if data is ready -->
        document.getElementById("Dhum_ID").innerHTML = this.responseText; <!-- Finding html ID what to replace -->
      }
    };
     Htp_Req.open("GET","/Dhum_ID",true);                                 <!-- Getting the Data prepared by the hardware(see void setup)-->
     Htp_Req.send();                                                      <!-- send data -->
  }, 5000);

  setInterval(function(){
    var Htp_Req = new XMLHttpRequest();                                   <!-- requesting data -->
     Htp_Req.onreadystatechange = function(){                             <!-- preparing data  -->
      if(this.readyState == 4 && this.status == 200){                     <!-- checking if data is ready -->
        document.getElementById("Chum_ID").innerHTML = this.responseText; <!-- Finding html ID what to replace -->
      }
    };
     Htp_Req.open("GET","/Chum_ID",true);                                 <!-- Getting the Data prepared by the hardware(see void setup)-->
     Htp_Req.send();                                                      <!-- send data -->
  }, 5000);

  setInterval(function(){
    var Htp_Req = new XMLHttpRequest();                                   <!-- requesting data -->
     Htp_Req.onreadystatechange = function(){                             <!-- preparing data  -->
      if(this.readyState == 4 && this.status == 200){                     <!-- checking if data is ready -->
        document.getElementById("Bhum_ID").innerHTML = this.responseText; <!-- Finding html ID what to replace -->
      }
    };
     Htp_Req.open("GET","/Bhum_ID",true);                                 <!-- Getting the Data prepared by the hardware(see void setup)-->
     Htp_Req.send();                                                      <!-- send data -->
  }, 5000);

  setInterval(function(){
    var Htp_Req = new XMLHttpRequest();                                   <!-- requesting data -->
     Htp_Req.onreadystatechange = function(){                             <!-- preparing data  -->
      if(this.readyState == 4 && this.status == 200){                     <!-- checking if data is ready -->
        document.getElementById("Ahum_ID").innerHTML = this.responseText; <!-- Finding html ID what to replace -->
      }
    };
     Htp_Req.open("GET","/Ahum_ID",true);                                 <!-- Getting the Data prepared by the hardware(see void setup)-->
     Htp_Req.send();                                                      <!-- send data -->
  }, 5000);

<!-- -----REFRESHING TEMPERATURE W/O Refreshing Webpage------ -->
  setInterval(function(){
    var Htp_Req = new XMLHttpRequest();                                   <!-- requesting data -->
     Htp_Req.onreadystatechange = function(){                             <!-- preparing data  -->
      if(this.readyState == 4 && this.status == 200){                     <!-- checking if data is ready -->
        document.getElementById("Etemp_ID").innerHTML = this.responseText; <!-- Finding html ID what to replace -->
      }
    };
     Htp_Req.open("GET","/Etemp_ID",true);                                 <!-- Getting the Data prepared by the hardware(see void setup)-->
     Htp_Req.send();                                                      <!-- send data -->
  }, 5000);

  setInterval(function(){
    var Htp_Req = new XMLHttpRequest();                                   <!-- requesting data -->
     Htp_Req.onreadystatechange = function(){                             <!-- preparing data  -->
      if(this.readyState == 4 && this.status == 200){                     <!-- checking if data is ready -->
        document.getElementById("Dtemp_ID").innerHTML = this.responseText; <!-- Finding html ID what to replace -->
      }
    };
     Htp_Req.open("GET","/Dtemp_ID",true);                                 <!-- Getting the Data prepared by the hardware(see void setup)-->
     Htp_Req.send();                                                      <!-- send data -->
  }, 5000);

  setInterval(function(){
    var Htp_Req = new XMLHttpRequest();                                   <!-- requesting data -->
     Htp_Req.onreadystatechange = function(){                             <!-- preparing data  -->
      if(this.readyState == 4 && this.status == 200){                     <!-- checking if data is ready -->
        document.getElementById("Ctemp_ID").innerHTML = this.responseText; <!-- Finding html ID what to replace -->
      }
    };
     Htp_Req.open("GET","/Ctemp_ID",true);                                 <!-- Getting the Data prepared by the hardware(see void setup)-->
     Htp_Req.send();                                                      <!-- send data -->
  }, 5000);

  setInterval(function(){
    var Htp_Req = new XMLHttpRequest();                                   <!-- requesting data -->
     Htp_Req.onreadystatechange = function(){                             <!-- preparing data  -->
      if(this.readyState == 4 && this.status == 200){                     <!-- checking if data is ready -->
        document.getElementById("Btemp_ID").innerHTML = this.responseText; <!-- Finding html ID what to replace -->
      }
    };
     Htp_Req.open("GET","/Btemp_ID",true);                                 <!-- Getting the Data prepared by the hardware(see void setup)-->
     Htp_Req.send();                                                      <!-- send data -->
  }, 5000);

  setInterval(function(){
    var Htp_Req = new XMLHttpRequest();                                   <!-- requesting data -->
     Htp_Req.onreadystatechange = function(){                             <!-- preparing data  -->
      if(this.readyState == 4 && this.status == 200){                     <!-- checking if data is ready -->
        document.getElementById("Atemp_ID").innerHTML = this.responseText; <!-- Finding html ID what to replace -->
      }
    };
     Htp_Req.open("GET","/Atemp_ID",true);                                 <!-- Getting the Data prepared by the hardware(see void setup)-->
     Htp_Req.send();                                                      <!-- send data -->
  }, 5000);

 <!-- -----REFRESHING SOIL W/O Refreshing Webpage------ -->

   setInterval(function(){
    var Htp_Req = new XMLHttpRequest();                                   <!-- requesting data -->
     Htp_Req.onreadystatechange = function(){                             <!-- preparing data  -->
      if(this.readyState == 4 && this.status == 200){                     <!-- checking if data is ready -->
        document.getElementById("ESoil_ID").innerHTML = this.responseText; <!-- Finding html ID what to replace -->
      }
    };
     Htp_Req.open("GET","/ESoil_ID",true);                                 <!-- Getting the Data prepared by the hardware(see void setup)-->
     Htp_Req.send();                                                      <!-- send data -->
  }, 5000);

  setInterval(function(){
    var Htp_Req = new XMLHttpRequest();                                   <!-- requesting data -->
     Htp_Req.onreadystatechange = function(){                             <!-- preparing data  -->
      if(this.readyState == 4 && this.status == 200){                     <!-- checking if data is ready -->
        document.getElementById("DSoil_ID").innerHTML = this.responseText; <!-- Finding html ID what to replace -->
      }
    };
     Htp_Req.open("GET","/DSoil_ID",true);                                 <!-- Getting the Data prepared by the hardware(see void setup)-->
     Htp_Req.send();                                                      <!-- send data -->
  }, 5000);

  setInterval(function(){
    var Htp_Req = new XMLHttpRequest();                                   <!-- requesting data -->
     Htp_Req.onreadystatechange = function(){                             <!-- preparing data  -->
      if(this.readyState == 4 && this.status == 200){                     <!-- checking if data is ready -->
        document.getElementById("CSoil_ID").innerHTML = this.responseText; <!-- Finding html ID what to replace -->
      }
    };
     Htp_Req.open("GET","/CSoil_ID",true);                                 <!-- Getting the Data prepared by the hardware(see void setup)-->
     Htp_Req.send();                                                      <!-- send data -->
  }, 5000);

    setInterval(function(){
    var Htp_Req = new XMLHttpRequest();                                   <!-- requesting data -->
     Htp_Req.onreadystatechange = function(){                             <!-- preparing data  -->
      if(this.readyState == 4 && this.status == 200){                     <!-- checking if data is ready -->
        document.getElementById("BSoil_ID").innerHTML = this.responseText; <!-- Finding html ID what to replace -->
      }
    };
     Htp_Req.open("GET","/BSoil_ID",true);                                 <!-- Getting the Data prepared by the hardware(see void setup)-->
     Htp_Req.send();                                                      <!-- send data -->
  }, 5000);

    setInterval(function(){
    var Htp_Req = new XMLHttpRequest();                                   <!-- requesting data -->
     Htp_Req.onreadystatechange = function(){                             <!-- preparing data  -->
      if(this.readyState == 4 && this.status == 200){                     <!-- checking if data is ready -->
        document.getElementById("ASoil_ID").innerHTML = this.responseText; <!-- Finding html ID what to replace -->
      }
    };
     Htp_Req.open("GET","/ASoil_ID",true);                                 <!-- Getting the Data prepared by the hardware(see void setup)-->
     Htp_Req.send();                                                      <!-- send data -->
  }, 5000);
</script>
</html>)rawliteral";

//Processor. will check %TEMPERATURE% or %HUMIDITY%. so it can place data respectively
// TEMPERATURE  
  String processor(const String& var){
  if(var == "----"){
    return ATemp();
  }
  else if(var =="----"){
    return BTemp();
  }
  else if(var =="----"){
    return CTemp();
  }
  else if(var =="----"){
    return DTemp();
  }
  else if(var =="----"){
    return ETemp();
  }
  // HUMIDITY
  else if(var =="----"){
    return AHum();
  }
  else if(var =="----"){
    return BHum();
  }
  else if(var =="----"){
    return CHum();
  }
  else if(var =="----"){
    return DHum();
  }
  else if(var =="----"){
    return EHum();
  }
  //SOIL
  else if(var =="----"){
    return ASoil();
  }
  else if(var =="----"){
    return BSoil();
  }
  else if(var =="----"){
    return CSoil();
  }
  else if(var =="----"){
    return DSoil();
  }
  else if(var =="----"){
    return ESoil();
  }
  return String();

}



void setup() {
  // ---------- SETTING UP ESP 32 WIFI --------------///
  Serial.begin(115200);
  dht.begin();
  pinMode(waterpump, OUTPUT);
  digitalWrite(waterpump,HIGH);
  //-- SOFTAPI --
  WiFi.softAP(ssid,password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("Soft API Address: ");
  Serial.println(myIP);
  Serial.println("Server Started");


  //-----------Running the Hardware------------- //
     //--------Check what data should replace. request path---//
 //TEMPERATURE
  server.on("/",HTTP_GET,[](AsyncWebServerRequest *request){
    request -> send_P(200,"text/html", index_html, processor);
  });
  server.on("/Etemp_ID",[](AsyncWebServerRequest *request){
    request -> send_P(200,"text/plain", ETemp().c_str());
  });
  server.on("/Ehum_ID",[](AsyncWebServerRequest *request){
    request -> send_P(200,"text/plain", EHum().c_str());
  });
  server.on("/ESoil_ID",[](AsyncWebServerRequest *request){
    request -> send_P(200,"text/plain", ESoil().c_str());
  });
  server.on("/Dtemp_ID",[](AsyncWebServerRequest *request){
    request -> send_P(200,"text/plain", DTemp().c_str());
  });
  server.on("/Dhum_ID",[](AsyncWebServerRequest *request){
    request -> send_P(200,"text/plain", DHum().c_str());
  });
  server.on("/DSoil_ID",[](AsyncWebServerRequest *request){
    request -> send_P(200,"text/plain", DSoil().c_str());
  });
  server.on("/Ctemp_ID",[](AsyncWebServerRequest *request){
    request -> send_P(200,"text/plain", CTemp().c_str());
  });
  server.on("/Chum_ID",[](AsyncWebServerRequest *request){
    request -> send_P(200,"text/plain", CHum().c_str());
  });
  server.on("/CSoil_ID",[](AsyncWebServerRequest *request){
    request -> send_P(200,"text/plain", CSoil().c_str());
  });
  server.on("/Btemp_ID",[](AsyncWebServerRequest *request){
    request -> send_P(200,"text/plain", BTemp().c_str());
  });
  server.on("/Bhum_ID",[](AsyncWebServerRequest *request){
    request -> send_P(200,"text/plain", BHum().c_str());
  });
  server.on("/BSoil_ID",[](AsyncWebServerRequest *request){
    request -> send_P(200,"text/plain", BSoil().c_str());
  });
  server.on("/Atemp_ID",[](AsyncWebServerRequest *request){
    request -> send_P(200,"text/plain", ATemp().c_str());
  });
  server.on("/Ahum_ID",[](AsyncWebServerRequest *request){
    request -> send_P(200,"text/plain", AHum().c_str());
  });
  server.on("/ASoil_ID",[](AsyncWebServerRequest *request){
    request -> send_P(200,"text/plain", ASoil().c_str());
  });
 //HUMIDITY







  //SOIL SENSOR



server.begin();

}

void loop() {
  // put your main code here, to run repeatedly:
}