import React from 'react';
import ReactDOM from 'react-dom';
import './index.css';
import App from './App';
import * as serviceWorker from './serviceWorker';
import { connect } from 'mqtt';
const client  = connect('ws://localhost:9001');

client.on('connect', function () {
    console.log('successfully connected to broker');
    client.subscribe('ofxLiveSet/mqttSynchroniser/#', function (err) {
      if (!err) {
      }
    })
  })
client.on("message", function (topic, payload) {
  console.log([topic, payload].join(": "))
//   TODO: dispatch redux action
})


ReactDOM.render(<App />, document.getElementById('root'));

// If you want your app to work offline and load faster, you can change
// unregister() to register() below. Note this comes with some pitfalls.
// Learn more about service workers: http://bit.ly/CRA-PWA
serviceWorker.unregister();
