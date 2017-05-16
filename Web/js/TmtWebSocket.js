/**
 * Created by rwp on 2016/7/15.
 */
//初始化：
// TmtWebSocket.Http("222.28.39.64:9002");
//发送数据
// TmtWebSocket.sendMsg(canvas.toDataURL('image/jpeg'));
//

var TmtWs;
var TmtWebSocket =
{
  http: function (Url) {
    TmtWs = new WebSocket("ws://"+Url);
    TmtWs.onopen = function () {
      TmtWs.send("Hello World");
    };
    TmtWs.onmessage = function (evt) {
      TmtWebSocket.onMsg(evt.data);
    }
  },
  https: function (Url) {
    var ws = new WebSocket("wss://"+Url);
  },
  doSend:function (Data) {
    TmtWs.send(Data);
  },
  onMsg:function (Msg){
    console.log(Msg);
  }
};
