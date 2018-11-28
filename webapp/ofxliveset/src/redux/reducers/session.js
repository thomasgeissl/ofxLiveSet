const trackTopic = "ofxLiveSet/mqttSynchroniser/session/trackName/parameters/";
const trackIoTopic = "ofxLiveSet/mqttSynchroniser/session/trackName/ioParameters/";
const clipTopic = "ofxLiveSet/mqttSynchroniser/session/trackName/clipName/parameters/";

const trackTopicParts = trackTopic.split('/')
const trackIoTopicParts = trackIoTopic.split('/')
const clipTopicParts = clipTopic.split('/')

// TODO: regex
const isTrack = (id) => {
  const idParts = id.split('/')
  if(idParts.length === trackTopicParts.length){
    if(idParts[0] === trackTopicParts[0] && idParts[1] === trackTopicParts[1] && idParts[2] === trackTopicParts[2] && idParts[4] === trackTopicParts[4]){
      return true;
    }
  }
  return false;
}
const isTrackIo = (id) => {
  const idParts = id.split('/')
  if(idParts.length === trackIoTopicParts.length){
    if(idParts[0] === trackIoTopicParts[0] && idParts[1] === trackIoTopicParts[1] && idParts[2] === trackIoTopicParts[2] && idParts[4] === trackIoTopicParts[4]){
      return true;
    }
  }
  return false;
}
const isClip = (id) => {
  const idParts = id.split('/')
  if(idParts.length === clipTopicParts.length){
    if(idParts[0] === clipTopicParts[0] && idParts[1] === clipTopicParts[1] && idParts[2] === clipTopicParts[2] && idParts[5] === clipTopicParts[5]){
      return true;
    }
  }
  return false;
}
const getTrackName = (id) => {
  const idParts = id.split('/')
  return idParts[3]
}

// TODO: data structure for tracks? array, map? redux best practices?
const initialState = {
    tracks: [],
    counter: 0
}

const getTrack = (tracks, name) => {
  tracks.forEach(element => {
    if(element.name === name){
      return element;
    }
  });
  return {}
}
export default (state = initialState, action) => {
    switch (action.type) {
     case 'SETPARAMETER':
     const id = action.payload.id
      if(isTrack(id)){
        const tracks = Object.assign(state.tracks)
        tracks[getTrackName(id)] = {name: getTrackName(id)}
        return Object.assign({}, state, {tracks: tracks, counter: state.counter+1})
      }else if(isTrackIo(id)){
      }else if(isClip(id)){

     }
     return state
     default:
      return state
    }
   }