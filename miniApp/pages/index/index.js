//index.js
//获取应用实例
var app = getApp();
var that;
Page({
  data: {
    identityImageSrc:"http://pic.baike.soso.com/p/20130821/20130821215447-1728773261.jpg",
    selfieSrc:"http://www.qulishi.com/UploadFile/2014-8/2014812150860.jpg",
    allTv: []
  },
  // goToSearchPage: function () {
  //   wx.navigateTo({
  //     url: '../search/search'
  //   })
  // },
    addIdentifyImage:function () {

        wx.chooseImage({
            count: 1, // 默认9
            sizeType: ['original', 'compressed'], // 可以指定是原图还是压缩图，默认二者都有
            sourceType: ['album', 'camera'], // 可以指定来源是相册还是相机，默认二者都有
            success: function (res) {
                // 返回选定照片的本地文件路径列表，tempFilePath可以作为img标签的src属性显示图片
                var tempFilePaths = res.tempFilePaths;
                console.log(tempFilePaths);
                that.setData({
                    identityImageSrc:tempFilePaths[0]
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
                console.log(tempFilePaths);
                that.setData({
                    selfieSrc:tempFilePaths[0]
                })
            }
        })
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
