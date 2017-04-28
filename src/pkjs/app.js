// Constant variables
var API          = "https://a.wunderlist.com/api/v1";
var CLIENTID     = "aad17840a957f1761637";
var REPORTINGURL = "https://wunderpebble.com/config/report.php";
var VERSION      = 0.1;
var DEBUG        = true;

// Load settings and credentials from LocalStorage
var token        = localStorage.getItem("token") || false;
var reporting    = localStorage.getItem("reporting") || true;

// Initialize arrays to process lists & tasks
var lists = [];
var tasks = [];

// Stacks to process asynchronous ajax calls
var toLoad  = [];
var loading = [];
var loaded  = [];

// Data to be sent back to app
var nextTask     = false;
var taskCount    = 0;
var taskDate     = '';
var taskReminder = '';
var taskNote     = '';
var taskColor    = '';

// Workflow
// getLists() -> getTasks() -> getNextTask() -> getReminder() -> getNote() -> getNextTaskDate() -> getNextTaskColor() -> sendData()

/**
 * Get a list of the user's lists
 */
function getLists()
{
	if ( DEBUG ) console.log( 'getLists()' );

	if ( !token )
	{
		Pebble.sendAppMessage(
		{
			'0': 'Setup Needed',
			'1': 'Visit App Settings',
			'3': 'red'
		} );

		return;
	}

	var req = new XMLHttpRequest();

	req.open( 'GET', API + '/lists', true );
	req.setRequestHeader( "X-Access-Token", token );
	req.setRequestHeader( "X-Client-ID", CLIENTID );
	req.setRequestHeader( "Content-Type", "application/json" );

	req.onload = function ()
	{
		if ( req.readyState === 4 )
		{
			if ( req.status === 200 )
			{
				var listObj = JSON.parse( req.responseText );

				lists = [];

				for ( var i = 0; i < listObj.length; i++ )
				{
					lists.push( listObj[ i ].id );
				}

				if ( DEBUG ) console.log( lists.length + ' lists' );

				toLoad = lists;
				loading = [];
				loaded = [];
				tasks = {};

				getTasks();
			}
			else
			{
				console.log( 'Error getting lists' );
				console.log( req.responseText );

				Pebble.sendAppMessage(
				{
					'0': '',
					'1': 'Error!'
				} );
			}
		}
	};

	req.send( null );
}

/**
 * Get all incomplete tasks for the user
 */
function getTasks()
{
	// 	if(DEBUG) console.log('getTasks()');

	if ( toLoad.length && loading.length < 5 )
	{
		var listID = toLoad.shift();
		loading.push( listID );
		//console.log(listID);

		var req = new XMLHttpRequest();

		req.open( 'GET', API + '/tasks?list_id=' + listID, true );
		req.setRequestHeader( "X-Access-Token", token );
		req.setRequestHeader( "X-Client-ID", CLIENTID );
		req.setRequestHeader( "Content-Type", "application/json" );

		req.onload = function ()
		{
			if ( req.readyState === 4 )
			{
				if ( req.status === 200 )
				{
					var taskObj = JSON.parse( req.responseText );

					for ( var j = 0; j < taskObj.length; j++ )
					{
						if ( taskObj[ j ].completed === false && taskObj[ j ].hasOwnProperty( 'due_date' ) && taskObj[ j ].due_date )
						{
							tasks[ taskObj[ j ].id ] = {
								id: taskObj[ j ].id,
								title: taskObj[ j ].title,
								due_date: taskObj[ j ].due_date,
								created_at: taskObj[ j ].created_at
							};
						}
					}
				}
				else
				{
					console.log( 'Error getting tasks' );
					console.log( req.responseText );

					Pebble.sendAppMessage(
					{
						'0': '',
						'1': 'Error!'
					} );
				}

				loaded.push( loading.splice( loading.indexOf( listID ), 1 )[ 0 ] );

				// console.log('To Load: '+toLoad.length+' Loading: '+loading.length+' Loaded: '+loaded.length);

				if ( toLoad.length === 0 && loading.length === 0 )
				{
					console.log( 'Sending Data' );
					localStorage.setItem( tasks, JSON.stringify( tasks ) );

					getNextTask();
				}
				else if ( loading.length < 5 )
				{
					getTasks();
				}
			}
		};

		req.send( null );

		if ( loading.length < 5 )
		{
			getTasks();
		}
	}
	else
	{

	}
}

/**
 * Get a task's reminder date
 */
function getReminder()
{
	if ( DEBUG ) console.log( 'getReminder()' );

	var req = new XMLHttpRequest();

	req.open( 'GET', API + '/reminders?task_id=' + nextTask.id, true );
	req.setRequestHeader( "X-Access-Token", token );
	req.setRequestHeader( "X-Client-ID", CLIENTID );
	req.setRequestHeader( "Content-Type", "application/json" );

	req.onload = function ()
	{
		if ( req.readyState === 4 )
		{
			if ( req.status === 200 )
			{
				var reminderObj = JSON.parse( req.responseText );

				if ( reminderObj.length )
					taskReminder = reminderObj[ 0 ].date;
				else
					taskReminder = '';

				getNote();
			}
			else
			{
				console.log( 'Error getting reminder for ' + nextTask.id );
				console.log( req.responseText );

				Pebble.sendAppMessage(
				{
					'0': '',
					'1': 'Error!'
				} );
			}
		}
	};

	req.send( null );
}

/**
 * Get a task's note
 */
function getNote()
{
	if ( DEBUG ) console.log( 'getNote()' );

	var req = new XMLHttpRequest();

	req.open( 'GET', API + '/notes?task_id=' + nextTask.id, true );
	req.setRequestHeader( "X-Access-Token", token );
	req.setRequestHeader( "X-Client-ID", CLIENTID );
	req.setRequestHeader( "Content-Type", "application/json" );

	req.onload = function ()
	{
		if ( req.readyState === 4 )
		{
			if ( req.status === 200 )
			{
				var noteObj = JSON.parse( req.responseText );

				if ( noteObj.length )
					taskNote = noteObj[ 0 ].content;
				else
					taskNote = '';

				getNextTaskDate();
				getNextTaskColor();
				sendData();
			}
			else
			{
				console.log( 'Error getting note' );
				console.log( req.responseText );

				Pebble.sendAppMessage(
				{
					'0': '',
					'1': 'Error!'
				} );
			}
		}
	};

	req.send( null );
}

/**
 * Determine next upcoming task from all tasks
 */
function getNextTask()
{
	if ( DEBUG ) console.log( 'getNextTask()' );

	var today = String( new Date( new Date().toLocaleDateString() ).toISOString() ).substring( 0, 10 );

	taskCount = 0;

	// Try to get a task due today
	for ( var id in tasks )
	{
		if ( tasks[ id ].due_date == today )
		{
			nextTask = tasks[ id ];
		}

		// Get count of tasks due today or in past
		if ( tasks[ id ].due_date < today || tasks[ id ].due_date == today )
		{
			taskCount++;
		}
	}

	// If we could not find a current task, use a backup strategy
	if ( !nextTask )
	{
		for ( var id in tasks )
		{
			if ( !nextTask )
			{
				nextTask = tasks[ id ];
			}
			else if ( tasks[ id ].due_date < nextTask.due_date )
			{
				nextTask = tasks[ id ];
			}
			else if ( tasks[ id ].due_date == nextTask.due_date && new Date( tasks[ id ].created_at ) < new Date( nextTask.created_at ) )
			{
				nextTask = tasks[ id ];
			}
		}
	}

	if ( !nextTask )
	{
		nextTask = false;
		taskDate = '';
		taskReminder = '';
		taskNote = '';
		taskColor = '';

		sendData();
	}
	else
	{
		getReminder();
	}
}

function getNextTaskDate()
{
	if ( DEBUG ) console.log( 'getNextTaskDate()' );

	var today = String( new Date( new Date().toLocaleDateString() ).toISOString() ).substring( 0, 10 );

	if ( nextTask.due_date === today )
	{
		if ( taskReminder && taskReminder.substring( 0, 10 ) == today )
		{
			var reminderDate = new Date( taskReminder );
			var hour = reminderDate.getHours();
			var minute = reminderDate.getMinutes();
			var period = 'AM';

			if ( hour > 12 )
			{
				hour = hour - 12;
				period = 'PM';
			}

			if ( minute < 10 )
			{
				minute = '0' + minute;
			}

			taskDate = "Due Today @ " + hour + ':' + minute + ' ' + period;
		}
		else
			taskDate = "Due Today";
	}
	else if ( nextTask.due_date === new Date( new Date().getTime() + 86400000 ).toJSON().substring( 0, 10 ) )
	{
		taskDate = "Due Tomorrow";
	}
	else if ( nextTask.due_date === new Date( new Date().getTime() - 86400000 ).toJSON().substring( 0, 10 ) )
	{
		taskDate = "Due Yesterday";
	}
	else if ( !nextTask )
	{
		taskDate = "";
	}
	else
	{
		var dateParts = nextTask.due_date.split( '-' );
		taskDate = 'Due ' + Number( dateParts[ 1 ] ) + '/' + Number( dateParts[ 2 ] ) + '/' + dateParts[ 0 ];
	}
}

function getNextTaskColor()
{
	if ( DEBUG ) console.log( 'getNextTaskColor()' );

	var today = String( new Date().toISOString() ).substring( 0, 10 );

	if ( nextTask.due_date < today )
	{
		taskColor = 'red';
	}
	else
	{
		taskColor = 'blue';
	}
}

function sendData()
{
	if ( DEBUG ) console.log( 'sendData()' );
	console.log( 'Due: ' + taskDate );
	console.log( 'Desc: ' + nextTask.title );
	console.log( 'Count: ' + taskCount );
	console.log( 'Color: ' + taskColor );

	Pebble.sendAppMessage(
	{
		'0': taskDate,
		'1': ( nextTask ) ? nextTask.title + "\n" + taskNote : "",
		'2': taskCount,
		'3': taskColor
	} );
}

Pebble.addEventListener( 'ready', function ( e )
{
	console.log( 'JS Ready' );

	getLists();
} );

Pebble.addEventListener( 'appmessage', function ( e )
{
	console.log( 'App Message Request' );

	getLists();
} );

Pebble.addEventListener( 'showConfiguration', function ( e )
{
	console.log( 'Opening Settings' );

	var data = {
		token: token,
		reporting: reporting,
		ring: 'seconds',
		color: 'red'
	};

	data = encodeURIComponent( JSON.stringify( data ) );

	console.log( data );

	var url = 'https://wunderpebble.com/config?app=wundertime&data=' + data;

	Pebble.openURL( url );
} );

Pebble.addEventListener( 'webviewclosed', function ( e )
{
	console.log( 'Settings Closed' );
	console.log( JSON.stringify( e.response ) );

	try
	{
		var data = JSON.parse( decodeURIComponent( e.response ) );

		// Save to Local Storage
		localStorage.setItem( "token", data.token );
		localStorage.setItem( "reporting", data.reporting );
		
		// Set variables
		token = data.token;
		reporting = data.reporting;
	}
	catch( err )
	{
		reportError( 'Saving Settings: ' + err );
		token = '490701ab3617f1dfc8280417a588d4622fabd8035fde176753be1367ff3a';
	}

} );

function reportError( err )
{
	if ( DEBUG ) console.log( "Reporting Error" );
	var today = new Date();

	if( reporting )
	{
		var data =
		+ '?timestamp=' + today.toISOString()
		+ '&error=' + err
		+ '&identifier=' + 1
		+ '&version=' + VERSION;

		var req = new XMLHttpRequest();

		req.open( 'GET', REPORTINGURL + data, true );

		req.onload = function ()
		{
			if ( req.readyState === 4 )
			{
				if ( req.status === 200 )
				{
					if ( DEBUG ) console.log( "Error Reported: " + err );
					if ( DEBUG ) console.log( req.responseText );
					return true;
				}
				else
				{
					console.log( 'Error reporting error?!?!?!' );
					console.log( req.responseText );
					return false;
				}
			}
		};

		req.send( null );
	}
	else
	{
		return false;
	}
}