import React from 'react';
import ReactDOM from 'react-dom';
import { Provider } from "react-redux";

import './index.css';
import * as serviceWorker from './serviceWorker';
import store from './redux/store';

import { connect } from 'mqtt';

import App from './components/App';
import Session from './components/Session';

const client  = connect('ws://localhost:9001');


client.on('connect', function () {
    console.log('successfully connected to broker');
    client.subscribe('ofxLiveSet/mqttSynchroniser/#', function (err) {
      if (!err) {
      }
    })
    client.publish('remote/ofxLiveSet/mqttSynchroniser/request', '{}');
})

client.on("message", function (topic, payload) {
  // console.log([topic, payload].join(": "))
  store.dispatch(
    {
      type: "SETPARAMETER",
      payload: {
        id: topic,
        payload
      }
    }
  )
})
// store.subscribe(() => {console.log(store.getState())})

ReactDOM.render(
  <Provider store={store}>
    <Session></Session>
  </Provider>, 
  document.getElementById('root')
  );
serviceWorker.register();
