var Eotu = {
	Socket: null,
	SocketId: null,
	console: console || {
		log: function () {
			return false;
		}
	},
	Connect: function (host, port, callback) {
		if (!this.Socket) {
			this.console.log('未初始化 Socket 对象');
			return false;
		}
		this.SocketId = this.Socket.connect(host, port, callback);
		return this.SocketId;
	},
	Send: function (data) {
		if (!this.SocketId) {
			this.console.log('未连接到服务器');
			return false;
		}
		return this.Socket.send(this.SocketId, data);
	},
	addEvent: function (name, func) {
		if (!this.Socket) {
			this.console.log('未初始化 Socket 对象');
			return false;
		}
		if (this.Socket.attachEvent) {
			this.Socket.attachEvent("on" + name, func);
		} else {
			this.Socket.addEventListener(name, func, false);
		}
	},
	WindowStyle: {
		None: 'None',
		SingleBorderWindow: 'SingleBorderWindow',
		ThreeDBorderWindow: 'ThreeDBorderWindow',
		ToolWindow: 'ToolWindow'
	},
	ResizeMode: {
		CanMinimize: 'CanMinimize',
		CanResize: 'CanResize',
		CanResizeWithGrip: 'CanResizeWithGrip'
	},
	AjaxGet: function (url) {
		this.Call('AjaxGet', {url: url});
	},
	ShowMessage: function (title, message) {
		this.Call('ShowMessage', {title: title, message: message});
	},
	SetWindowTitle: function (title) {
		this.Call('SetWindowTitle', {title: title});
	},
	SetWindowStyle: function (style) {
		this.Call('SetWindowStyle', {style: style});
	},
	SetResizeMode: function (mode) {
		this.Call('SetResizeMode', {mode: mode});
	},
	SetWindowSize: function (width, height) {
		this.Call('SetWindowSize', {width: width, height: height});
	},
	Init: function () {
		this.Socket = document.createElement("EMBED");
		this.Socket.setAttribute("type", "application/x-eotusocket");
		this.Socket.setAttribute("style", "width:0px;height:0px;");
		document.body.appendChild(this.Socket);
		return this.Socket;
	},
	Call: function (funName, json) {
		var event = new MessageEvent(funName, {
			'view': window,
			'bubbles': false,
			'cancelable': false,
			'data': JSON.stringify(json)
		});
		document.dispatchEvent(event);
	},
	Success: function (json) {
		alert(json);
	},
	Error: function (json) {
		alert(json);
	}
};

$(document).ready(function () {
	Eotu.SetWindowTitle('登录');
	Eotu.SetWindowSize(300, $(document).outerHeight(true));
	//Eotu.SetWindowStyle(Eotu.WindowStyle.None);
	//Eotu.AjaxGet("http://dev.eotu.com:81/api/front/index/hotelTypes");

	Eotu.Init();
	if (Eotu.Socket.valid) {
		Eotu.console.log('EotuSock 插件加载成功');
		$('#loginForm').submit(function() {
			var sockid;
			sockid = Eotu.Connect('192.168.1.108', 81, {
				'connected':function () {
					Eotu.Send("POST /api/local/account/auth HTTP/1.1\r\nHost: www.eotu.com:81\r\nConnection: Close\r\n\r\n" + $(this).formSerialize());
				},
				'change':function (code, status) {
					alert(status);
				},
				'receive':function (data) {
					alert(data);
				},
				'tcp':true
			});
			return false;
		});
	}
});