var that;
Page({
  data: {
    keyword: "",
    searchResult: [],
    loading:false,
    loadSuccess:true,
    afterSearch:false
  },
  onLoad: function () {
    that = this;
  },
  searchDouban: function (e) {   
    this.setData({
      keyword: e.detail.value.keyword,
      loading:true,
      afterSearch:false
    })
    wx.request({
      url: 'https://api.douban.com/v2/movie/search',
      data: {
        q: this.data.keyword
      },
      method: 'GET', // OPTIONS, GET, HEAD, POST, PUT, DELETE, TRACE, CONNECT
      header: {
        "Content-Type": "json"
      },
      success: function (res) {
        // success
        console.log(res);
        that.setData({
          searchResult: res.data.subjects,
          loadSuccess:true
        })
      },
      fail: function (res) {
        // fail
        console.log(res);
        that.setData({
          loadSuccess:false
        })
      },
      complete: function (res) {
        // complete
        console.log(res);
        that.setData({loading:false,afterSearch:true})
      }
    })
  },
  goToDetail:function(e){
    console.log(e);
    var url = "../detail/detail?id="+e.currentTarget.dataset.tvid;
    wx.navigateTo({
      url: url
    })
  },
    onShareAppMessage: function () {
    return {
      title: '极简追剧 追再多剧也不怕忘',
      path: '/pages/index/index'
    }
  }
})