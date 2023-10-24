from flask_login import UserMixin
from apps import db, login_manager
from apps.authentication.util import hash_pass
class Users(db.Model, UserMixin):
    __tablename__ = 'Users'
    id = db.Column(db.Integer, primary_key=True)
    username = db.Column(db.String(64), unique=True)
    email = db.Column(db.String(64), unique=True)
    password = db.Column(db.LargeBinary)
    def __init__(self, **kwargs):
        for property, value in kwargs.items():
            if hasattr(value, '__iter__') and not isinstance(value, str):
                value = value[0]
            if property == 'password':
                value = hash_pass(value)
            setattr(self, property, value)
    def __repr__(self):
        return str(self.username)
class Node1(db.Model,UserMixin):
    __tablename__ = 'Node1'
    id= db.Column(db.Integer,primary_key=True)
    nodeId=db.Column(db.Float,nullable=False)
    gatewayId=db.Column(db.Float,nullable=False)
    rssi=db.Column(db.Integer,nullable=False)
    snr=db.Column(db.Float,nullable=False)
    data1=db.Column(db.Float,nullable=False)
    data2=db.Column(db.Float,nullable=False)
    data3=db.Column(db.Float,nullable=False)
    data4=db.Column(db.Float,nullable=False)
    data5=db.Column(db.Float,nullable=False)
    data6=db.Column(db.Float,nullable=False)
    time=db.Column(db.DateTime, nullable=False)
    def __init__(self, **kwargs):
        for property, value in kwargs.items():
            if hasattr(value, '__iter__') and not isinstance(value, str):
                value = value[0]
            setattr(self, property, value)
    def __repr__(self):
        return str(self.nodeId)
class Node2(db.Model,UserMixin):
    __tablename__ = 'Node2'
    id= db.Column(db.Integer,primary_key=True)
    nodeId=db.Column(db.Float,nullable=False)
    gatewayId=db.Column(db.Float,nullable=False)
    rssi=db.Column(db.Integer,nullable=False)
    snr=db.Column(db.Float,nullable=False)
    data1=db.Column(db.Float,nullable=False)
    data2=db.Column(db.Float,nullable=False)
    time=db.Column(db.DateTime, nullable=False)
    def __init__(self, **kwargs):
        for property, value in kwargs.items():
            if hasattr(value, '__iter__') and not isinstance(value, str):
                value = value[0]
            setattr(self, property, value)
    def __repr__(self):
        return str(self.nodeId)
class CPU(db.Model,UserMixin):
    __tablename__ = 'CPU'
    id= db.Column(db.Integer,primary_key=True)
    procpu=db.Column(db.Float,nullable=False)
    appcpu=db.Column(db.Float,nullable=False)
    model=db.Column(db.String(64), nullable=False)
    cores=db.Column(db.Integer, nullable=False)
    features=db.Column(db.Integer, nullable=False)
    revision=db.Column(db.Integer, nullable=False)
    clkcpu=db.Column(db.Integer, nullable=False)
    xtalcpu=db.Column(db.Integer, nullable=False)
    time=db.Column(db.DateTime, nullable=False)
    def __init__(self, **kwargs):
        for property, value in kwargs.items():
            if hasattr(value, '__iter__') and not isinstance(value, str):
                value = value[0]
            setattr(self, property, value)
    def __repr__(self):
        return str(self.model)
class WiFi(db.Model,UserMixin):
    __tablename__ = 'WiFi'
    id= db.Column(db.Integer,primary_key=True)
    ssid=db.Column(db.String(128), nullable=False)
    scanmethod=db.Column(db.String(128), nullable=False)
    channel=db.Column(db.Integer, nullable=False)
    mac=db.Column(db.String(128), nullable=False)
    bssid=db.Column(db.String(128), nullable=False)
    sortmethod=db.Column(db.String(128), nullable=False)
    rssithr=db.Column(db.Integer, nullable=False)
    authmode=db.Column(db.String(128), nullable=False)
    protocol=db.Column(db.String(32), nullable=False)
    bandwidth=db.Column(db.String(32), nullable=False)
    rssireal=db.Column(db.Integer, nullable=False)
    time=db.Column(db.DateTime, nullable=False)
    def __init__(self, **kwargs):
        for property, value in kwargs.items():
            if hasattr(value, '__iter__') and not isinstance(value, str):
                value = value[0]
            setattr(self, property, value)
    def __repr__(self):
        return str(self.ssid)
class Memory(db.Model,UserMixin):
    __tablename__ = 'Memory'
    id= db.Column(db.Integer,primary_key=True)
    dramdata=db.Column(db.String(128), nullable=False)
    drambss=db.Column(db.String(128), nullable=False)
    iramtext=db.Column(db.String(128), nullable=False)
    iramvectors=db.Column(db.String(128), nullable=False)
    flashtext=db.Column(db.String(128), nullable=False)
    flashsize=db.Column(db.String(128), nullable=False)
    time=db.Column(db.DateTime, nullable=False)
    def __init__(self, **kwargs):
        for property, value in kwargs.items():
            if hasattr(value, '__iter__') and not isinstance(value, str):
                value = value[0]
            setattr(self, property, value)
    def __repr__(self):
        return str(self.ssid)
@login_manager.user_loader
def user_loader(id):
    return Users.query.filter_by(id=id).first()
@login_manager.request_loader
def request_loader(request):
    username = request.form.get('username')
    user = Users.query.filter_by(username=username).first()
    return user if user else None
