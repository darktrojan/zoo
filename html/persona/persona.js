var signinLink = document.getElementById('signin');
if (signinLink) {
	signinLink.onclick = function() {
		navigator.id.request();
		return false;
	};
}

var signoutLink = document.getElementById('signout');
if (signoutLink) {
	signoutLink.onclick = function() {
		navigator.id.logout();
		return false;
	};
}

var currentUser = null;
var cookies = document.cookie.split(';');
for (var i = 0; i < cookies.length; i++) {
	var cookie = cookies[i].trim();
	if (cookie) {
		var parts = cookie.split('=', 2);
		if (parts[0] == 'email') {
			currentUser = decodeURIComponent(parts[1]);
			break;
		}
	}
}

navigator.id.watch({
	loggedInUser: currentUser,
	onlogin: function(assertion) {
		XHR.post('/persona/signin.php', 'assertion=' + assertion, function(request) {
			if (request.responseText == 'login') {
				location.reload();
			} else {
				location.href = '/persona/new.php';
			}
		}, function() {
			navigator.id.logout();
		});
	},
	onlogout: function() {
		XHR.get('/persona/signout.php', function() {
			location.reload();
		});
	}
});
