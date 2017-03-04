//Tai-Hsien Ou Yang
//2017 Mar. 4th

function doGet(e){

var strLat = "",
    strLong = "",
    fltValue = "";
var dateTime = new Date();

  try {
 
    strLat = e.parameters.lat;
    strLong = e.parameters.long;
    fltValue = e.parameters.value;
    var sheetsObject = SpreadsheetApp.openByUrl("https://docs.google.com/spreadsheets/d/*****Google Sheet ID*****/edit"); //It's the URL of the speadsheet
    var dataLoggerSheet = sheetsObject.getSheetByName("logging");
    var intRow = dataLoggerSheet.getLastRow() + 1;
    
    dataLoggerSheet.getRange("A" + intRow).setValue(intRow -1);   // ID
    dataLoggerSheet.getRange("B" + intRow).setValue(dateTime);    // dateTime
    dataLoggerSheet.getRange("C" + intRow).setValue(strLat);      // latitude
    dataLoggerSheet.getRange("D" + intRow).setValue(strLong);     // longitude
    dataLoggerSheet.getRange("E" + intRow).setValue(fltValue);    // value  

    return ContentService.createTextOutput("\nlat: " + strLat + "\nlong: "+ strLong + "\nvalue: " + fltValue);
 
  } catch(error) { 

    Logger.log(error);    
    return ContentService.createTextOutput("error:" + error.message 
                                            + "\n" + new Date() );
  }  
}