import React, { Component } from "react";
import { connect } from "react-redux";

const mapStateToProps = (state) => {
    return {
        tracks: state.session.tracks,
        counter: state.session.counter
    };
};

class Session extends Component {
    render(){
        console.log(this.props.tracks)
        const tracks = this.props.tracks
        return (
        <ul>
            <li>{tracks.length}</li>
                {tracks.map((element, index) => {
                    return (<li key={ index }>{element.name}</li>)
                  })
                }
            </ul>
        )
    }
}
// const Session = ({ tracks, counter }) => (

//   );
export default connect(mapStateToProps, {})(Session);
 