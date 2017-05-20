/**
 * Created by wl on 2016/9/8.
 * Version: 1.0, 2016/9/8. Create.
 * Version: 1.1, 2016/9/12, Change para from html file path into string.
 */
/**
 * Require: jquery-xxx.min.js
 * Usage:
 *     var contentStr=
 *          "<p class=\"myNoBorder\">" +
 *          "<canvas id=image_layer" +" tabindex='0' alt='Image:'"+
 *          " name='body_section_view_canvas' class='myCanvas myNoBorder'" +
 *          " style='position:absolute; left:0; top:0; z-index:0;'></canvas>";
 *     var alert_busy_obj=alert_busy(contentStr);
 *     ...
 *     alert_busy_close(alert_busy_obj);
 */
/**
 * alert_busy: Initial a busy info defined by contentStr with a shadow top layer.
 * @param contentStr: Html format content sting.
 * @return : {'shadowDiv':shadowDiv,'alertBox':alertBox};
 */
function alert_busy(contentStr) {
    var top = document.body.scrollTop || document.documentElement.scrollTop;
    var sTop = document.documentElement.scrollTop || document.body.scrollTop;
    var sLeft = document.documentElement.scrollLeft || document.body.scrollLeft;
    var winSize = function(){
        var xScroll, yScroll, windowWidth, windowHeight, pageWidth, pageHeight;
        // innerHeight获取的是可视窗口的高度，IE不支持此属性
        if (window.innerHeight && window.scrollMaxY) {
            xScroll = document.body.scrollWidth;
            yScroll = window.innerHeight + window.scrollMaxY;
        } else if (document.body.scrollHeight > document.body.offsetHeight) { // all but Explorer Mac
            xScroll = document.body.scrollWidth;
            yScroll = document.body.scrollHeight;
        } else { // Explorer Mac...would also work in Explorer 6 Strict, Mozilla and Safari
            xScroll = document.body.offsetWidth;
            yScroll = document.body.offsetHeight;
        }

        if (self.innerHeight) {    // all except Explorer
            windowWidth = self.innerWidth;
            windowHeight = self.innerHeight;
        } else if (document.documentElement && document.documentElement.clientHeight) { // Explorer 6 Strict Mode
            windowWidth = document.documentElement.clientWidth;
            windowHeight = document.documentElement.clientHeight;
        } else if (document.body) { // other Explorers
            windowWidth = document.body.clientWidth;
            windowHeight = document.body.clientHeight;
        }

        // for small pages with total height less then height of the viewport
        if (yScroll < windowHeight) {
            pageHeight = windowHeight;
        } else {
            pageHeight = yScroll;
        }

        // for small pages with total width less then width of the viewport
        if (xScroll < windowWidth) {
            pageWidth = windowWidth;
        } else {
            pageWidth = xScroll;
        }

        return{
            'pageWidth':pageWidth,
            'pageHeight':pageHeight,
            'windowWidth':windowWidth,
            'windowHeight':windowHeight
        }
    }();
    //alert(winSize.pageWidth);
    //遮罩层
    var styleStr = 'top:0;left:0;position:absolute;z-index:10000;background:#aaaaaa;width:' + winSize.pageWidth + 'px;height:' +  (winSize.pageHeight + 30) + 'px;';
    styleStr += "opacity:0.8;"; //遮罩层DIV
    var shadowDiv = document.createElement('div'); //添加阴影DIV
    shadowDiv.style.cssText = styleStr; //添加样式
    shadowDiv.id = "shadowDiv";
    //如果是IE6则创建IFRAME遮罩SELECT

    document.body.insertBefore(shadowDiv, document.body.firstChild); //遮罩层加入文档
    //弹出框
    // var styleStr1 ='display:block;position:fixed;left:25%;top:20%;z-index:10001'; //弹出框的位置
    var styleStr1 ='display:block;position:fixed;left:5%;top:5%;width:90%;z-index:10001'; //弹出框的位置
    var alertBox = document.createElement('div');
    alertBox.id = 'alertBox';
    alertBox.style.cssText = styleStr1;
    // //创建弹出框里面的内容P标签
    document.body.appendChild(alertBox);
    document.getElementById("alertBox").innerHTML=contentStr;
    return{
        'shadowDiv':shadowDiv,
        'alertBox':alertBox
    }
}
/**
 * alert_busy_close: Close a busy info box initialled by alert_busy.
 * @param alert_busy_obj:{'shadowDiv':shadowDiv,'alertBox':alertBox};
 */
function alert_busy_close(alert_busy_obj) {
    if(alert_busy_obj.alertBox!=undefined){
        document.body.removeChild(alert_busy_obj.alertBox);
    }
    if(alert_busy_obj.shadowDiv!=undefined){
        document.body.removeChild(alert_busy_obj.shadowDiv);
    }
}