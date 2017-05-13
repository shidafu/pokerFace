// pages/detail/detail.js
var that;
Page({
  data: {
    id: -1,
    detail: {},
    following: false,
    index: -1,
    episodes: [],
    manual_episode_to_watch: "",
    loading:true
  },
  onLoad: function (options) {
    // 页面初始化 options为页面跳转所带来的参数
    console.log(options);
    this.setData({
      id: options.id,
      episodes: [],
      index: -1
    });
    that = this;
    wx.request({
      url: 'https://api.douban.com/v2/movie/subject/' + this.data.id,
      method: 'GET', // OPTIONS, GET, HEAD, POST, PUT, DELETE, TRACE, CONNECT
      header: {
        'Content-Type': 'json'
      },
      success: function (res) {
        // success
        //console.log(res);
        var episodes = [];
        for (var i = 0; i < res.data.episodes_count; i++) {
          episodes.push(i + 1);
        }
        that.setData({
          detail: res.data,
          episodes: episodes
        })
        console.log(that.data);
        var allTv = wx.getStorageSync('allTv') || [];
        for (var i = 0; i < allTv.length; i++) {
          if (that.data.id == allTv[i].id) {
            that.setData({
              following: true,
              index: allTv[i].episodeToWatch - 1,
              manual_episode_to_watch: allTv[i].episodeToWatch
            })
          }
        }
      },
      fail: function (res) {
      },
      complete: function (res) {
        that.setData({
          loading:false
        })
      }
    })
  },
  onReady: function () {
    // 页面渲染完成
  },
  onShow: function () {
    // 页面显示
  },
  onHide: function () {
    // 页面隐藏
  },
  onUnload: function () {
    // 页面关闭
  },
  setCurrentManualEpisode: function (e) {
    console.log(e);
    console.log(this.data);
    this.setData({
      manual_episode_to_watch: e.detail.value
    })
  },
  addTv: function () {
    var allTv = wx.getStorageSync('allTv') || []
    allTv.unshift({
      id: this.data.id,
      name: this.data.detail.title,
      imageUrl: this.data.detail.images.medium,
      episodeToWatch: "1",
      episodesCount: this.data.detail.episodes_count
    });
    wx.setStorageSync('allTv', allTv);
    wx.redirectTo({
      url: '/pages/detail/detail?id=' + this.data.id
    })
  },
  deleteTv: function () {
    var allTv = wx.getStorageSync('allTv') || []
    for (var i = 0; i < allTv.length; i++) {
      if (this.data.id == allTv[i].id) {
        allTv.splice(i, 1); // this is how to remove an item
        break;
      }
    }
    wx.setStorageSync('allTv', allTv);

    wx.navigateBack({
      delta: 5
    })
  },
  updateTv: function (e) {
    console.log("update tv");
    console.log(this.data);
    var allTv = wx.getStorageSync('allTv') || [];
    for (var i = 0; i < allTv.length; i++) {
      if (this.data.id == allTv[i].id) {
        if (this.data.episodes.length > 0) {
          allTv[i].episodeToWatch = this.data.episodes[this.data.index];
        } else {
          allTv[i].episodeToWatch = this.data.manual_episode_to_watch;
        }
        break;
      }
    }
    wx.setStorageSync('allTv', allTv);
    wx.navigateBack({
      delta: 5
    })
  },
  pickerChanged: function (e) {
    //改变index值，通过setData()方法重绘界面
    this.setData({
      index: e.detail.value
    });
  },
  onShareAppMessage: function () {
    return {
      title: '极简追剧 追再多剧也不怕忘',
      path: '/pages/index/index'
    }
  }
})