//index.js
//获取应用实例
var app = getApp();
var that;
Page({
    data: {
        identityImageSrc:"http://pic.baike.soso.com/p/20130821/20130821215447-1728773261.jpg",
        selfieSrc:"http://www.qulishi.com/UploadFile/2014-8/2014812150860.jpg",
        identityImageBase64:"",
        selfieBase64:"",
        response:{},
        phpUrl:"http://172.23.71.2/identity/imageToBase64.php",
        wssUrl:'wss://172.23.71.3:9002',
        gap:0.50
    },
    addIdentifyImage:function () {
        wx.chooseImage({
            count: 1, // 默认9
            sizeType: ['original', 'compressed'], // 可以指定是原图还是压缩图，默认二者都有
            sourceType: ['album', 'camera'], // 可以指定来源是相册还是相机，默认二者都有
            success: function (res) {
                // 返回选定照片的本地文件路径列表，tempFilePath可以作为img标签的src属性显示图片
                var tempFilePaths = res.tempFilePaths;
                //console.log(tempFilePaths);
                that.setData({
                    identityImageSrc:tempFilePaths[0]
                });

                wx.uploadFile({
                    url: that.data.phpUrl,
                    filePath: that.data.identityImageSrc,
                    name: 'fileA',
                    formData:{
                        'user': 'test'
                    },
                    success: function(res){
                        var data = res.data
                        //console.log(" received base64:",data);
                        that.setData({
                            identityImageBase64:data
                        })
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
                    selfieSrc:tempFilePaths[0]
                });

                wx.uploadFile({
                    url: that.data.phpUrl,
                    filePath: that.data.selfieSrc,
                    name: 'fileA',
                    formData:{
                        'user': 'test'
                    },
                    success: function(res){
                        var data = res.data
                        //console.log(" received base64:",data);
                        that.setData({
                            selfieBase64:data
                        })
                    }
                })

            }
        })
    },
    compare:function () {
        wx.showToast({
            title: '数据加载中',
            icon:'loading',
            duration:30000
        });
        var socketOpen = false
        // var socketMsgQueue = []
        wx.connectSocket({
            url: this.data.wssUrl
        });

        wx.onSocketOpen(function(res) {
            console.log('WebSocket连接已打开！')
            socketOpen = true;
            if (socketOpen) {
                console.log("sending socket")

                var tt = {
                    requestType : 'askFace',
                    recognitionType : "inTwo",
                    //pic1Data : 'ccdd',
                    pic1Data : that.data.identityImageBase64,
                    //pic2Data : 'aabb'};
                    pic2Data : that.data.selfieBase64};
                //console.log(tt);
                var last=JSON.stringify(tt);
                wx.sendSocketMessage({
                    data:last});

            }

        })
        wx.onSocketError(function(res){
            console.log('WebSocket连接打开失败，请检查！')
        })
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
    testResponse:function () {
        wx.hideLoading();
        var re = {
            "requestType" : "replyFace",
            "sourceHDL" : "0000asdff234",
            "resultState" : "ok",
            "resultValue" : "0.56"
        };
        var matchRate = re.resultValue*100
        wx.showModal({
            content: '匹配率：'+matchRate.toFixed(2)+'%',
            showCancel: false,
            success: function (res) {
                if (res.confirm) {
                    console.log('用户点击确定')
                }
            }
        });
    },
    onLoad: function () {
        console.log('onLoad');
        that = this;

    },
    onShow: function () {
        var allTv = wx.getStorageSync('allTv') || [];
        this.setData({
            allTv: allTv
        })
    },
    onShareAppMessage: function () {
        return {
            title: '',
            path: '/pages/index/index'
        }
    }
})
