//index.js
//获取应用实例
var app = getApp();
var that;
var socketOpen = false;
Page({
    data: {
        identityImageSrc:"http://pic.baike.soso.com/p/20130821/20130821215447-1728773261.jpg",
        selfieSrc:"http://www.qulishi.com/UploadFile/2014-8/2014812150860.jpg",
        identityImageBase64:"",
        selfieBase64:"",
        response:{},
        phpUrl:"https://pockerface.zhaopiano.cn/identity/imageToBase64.php",
        wssUrl:'wss://pockerface.zhaopiano.cn:9002',
        gap:0.50,
        disableCompare:"disabled"
    },
    addIdentifyImage:function () {
        wx.chooseImage({
            count: 1, // 默认9
            sizeType: ['compressed'], // 可以指定是原图还是压缩图，默认二者都有
            sourceType: ['album', 'camera'], // 可以指定来源是相册还是相机，默认二者都有
            success: function (res) {
                // 返回选定照片的本地文件路径列表，tempFilePath可以作为img标签的src属性显示图片
                var tempFilePaths = res.tempFilePaths;
                //console.log(tempFilePaths);
                that.setData({
                    identityImageSrc:tempFilePaths[0],
                    identityImageBase64:""
                });
                readyToCompare();
                wx.uploadFile({
                    url: that.data.phpUrl,
                    filePath: that.data.identityImageSrc,
                    name: 'fileA',
                    formData:{
                        'user': 'test'
                    },
                    success: function(res){
                        var data = res.data
                        wx.showToast({
                          title: 'base64加载成功！'
                        });
                        //console.log(" received base64:",data);
                        that.setData({
                            identityImageBase64:data
                        });
                        readyToCompare();
                    }
                })
            }
        })
    },
    addSelfie:function () {
        wx.chooseImage({
            count: 1, // 默认9
            sizeType: ['original', 'compressed'], // 可以指定是原图还是压缩图，默认二者都有
            sourceType: ['album', 'camera'], // 可以指定来源是相册还是相机，默认二者都有
            success: function (res) {
                // 返回选定照片的本地文件路径列表，tempFilePath可以作为img标签的src属性显示图片
                var tempFilePaths = res.tempFilePaths;
                //console.log(tempFilePaths);

                that.setData({
                    selfieSrc:tempFilePaths[0],
                    selfieBase64:""
                });
                readyToCompare();
                wx.uploadFile({
                    url: that.data.phpUrl,
                    filePath: that.data.selfieSrc,
                    name: 'fileA',
                    formData:{
                        'user': 'test'
                    },
                    success: function(res){
                        var data = res.data;
                        wx.showToast({
                            title: 'base64加载成功！'
                        });
                        //console.log(" received base64:",data);
                        that.setData({
                            selfieBase64:data
                        });
                        readyToCompare();
                    },
                    fail:function (e) {
                        console.log(e);
                        console.log("error uploading");
                    },
                    complete:function () {
                        console.log("uploading finished")
                    }
                })

            }
        })
    },
    compare:function () {


        wx.showToast({
            title: '数据加载中',
            icon:'loading',
            duration:60000,
            mask:true
        });

        if (socketOpen) {
            console.log("sending socket")
            var tt = {
                requestType : 'askFace',
                recognitionType : "inTwo",
                pic1Data : that.data.identityImageBase64,
                pic2Data : that.data.selfieBase64};
            var last=JSON.stringify(tt);
            wx.sendSocketMessage({
                data:last});
        }else{
            wx.showModal({
              title: '服务器出错，请稍后再试'
            })
        }

        wx.onSocketMessage(function(res) {
            console.log('收到服务器内容：' + res.data);
            var re = JSON.parse(res.data);
            wx.hideToast();
            var matchRate = re.resultValue*100;
            var modelContent = "";
            var matchResult = "";
            if(re.resultState == "ok"){
              if(re.resultValue > that.data.gap){
                matchResult ="匹配成功 "
              }else{
                matchResult = "匹配失败 "
              }
              modelContent = matchResult+ '匹配率：'+matchRate.toFixed(2)+'%'
            }else{
              modelContent = "出错，请重试！"
            }

            wx.showModal({
                content: modelContent,
                showCancel: false,
                success: function (res) {
                    if (res.confirm) {
                        console.log('用户点击确定')
                    }
                }
            });
        })
    },
    onLoad: function () {
        console.log('onLoad');
        that = this;
    },
    onShow: function () {
        if(!socketOpen) {
            wx.connectSocket({
                url: this.data.wssUrl
            });
            wx.onSocketOpen(function (res) {
                console.log('WebSocket连接已打开！')
                socketOpen = true;
            });
            wx.onSocketError(function(res){
                console.log('WebSocket连接打开失败，请检查！')
                console.log(res)
                var modelContent = res.errMsg;
                wx.showModal({
                    content: modelContent,
                    showCancel: false,
                    success: function (res) {
                        if (res.confirm) {
                            console.log('用户点击确定');
                        }
                    }
                });
            });
        }
    },
    onHide:function () {
        wx.closeSocket();
        socketOpen = false;
        console.log("WebSocket连接已关闭！")
    },
    onShareAppMessage: function () {
        return {
            title: '',
            path: '/pages/index/index'
        }
    }
});
function readyToCompare() {
    if (that.data.identityImageBase64.length > 0 && that.data.selfieBase64.length > 0){
        that.setData({
            disableCompare:""
        })
    }else {
        that.setData({
            disableCompare:"disabled"
        })
    }
}