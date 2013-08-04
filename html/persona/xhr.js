var XHR = {
	_makeRequest: function(aMethod, aURL, aSuccessCallback, aFailureCallback) {
		var request = new XMLHttpRequest();
		request._url = aURL;
		request.open(aMethod, aURL, true);
		request.onreadystatechange = function() {
			if (request.readyState == 4) {
				if (request.status == 200) {
					aSuccessCallback(request);
				} else if (request.status) {
					aFailureCallback(request);
				}
			}
		};
		request.onerror = function() {
			aFailureCallback(request);
		};
		return request;
	},
	get: function(aURL, aSuccessCallback, aFailureCallback) {
		if (!aSuccessCallback)
			aSuccessCallback = this.defaultCallback;
		if (!aFailureCallback)
			aFailureCallback = this.defaultCallback;

		var request = this._makeRequest('GET', aURL, aSuccessCallback, aFailureCallback);
		request.send(null);
	},
	post: function(aURL, aData, aSuccessCallback, aFailureCallback, aContentType) {
		if (!aSuccessCallback)
			aSuccessCallback = this.defaultCallback;
		if (!aFailureCallback)
			aFailureCallback = this.defaultCallback;
		if (!aContentType)
			aContentType = 'application/x-www-form-urlencoded';

		var request = this._makeRequest('POST', aURL, aSuccessCallback, aFailureCallback);
		request.setRequestHeader('Content-Type', aContentType);
		request.setRequestHeader('Content-Length', aData.length);
		request.setRequestHeader('Connection', 'close');
		request.send(aData);
	},
	postFormData: function(aURL, aData, aSuccessCallback, aFailureCallback) {
		if (!aSuccessCallback)
			aSuccessCallback = this.defaultCallback;
		if (!aFailureCallback)
			aFailureCallback = this.defaultCallback;

		var request = this._makeRequest('POST', aURL, aSuccessCallback, aFailureCallback);
		request.setRequestHeader('Connection', 'close');
		request.send(aData);
	},
	defaultCallback: function(aRequest) {
		var status = aRequest.status;
		var header = aRequest.getResponseHeader('Content-Type');
		var response = aRequest.responseText;

		if (status != 200 && console && 'error' in console)
			console.error('XHR to ' + aRequest._url + ' returned status ' + status);

		if (response) {
			if (header && header.indexOf('application/json') == 0)
				alert(JSON.parse(response));
			else
				alert(response);
		} else if (status != 200) {
			alert('An error has occurred.');
		}
	}
};
