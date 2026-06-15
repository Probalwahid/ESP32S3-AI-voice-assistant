# Backend Development Guide

Comprehensive guide for developing the Flask backend server.

## Architecture

### Technology Stack

- **Framework**: Flask / FastAPI
- **Language**: Python 3.9+
- **Audio Processing**: librosa, scipy
- **Machine Learning**: TensorFlow/PyTorch (optional)
- **Message Queue**: MQTT, WebSocket
- **Database**: SQLite / PostgreSQL

## Project Structure

```
backend/
├── app.py                  # Main Flask application
├── config.py              # Configuration management
├── requirements.txt       # Python dependencies
├── .env.example          # Environment variables
├── routes/
│   ├── audio.py          # Audio processing routes
│   ├── llm.py            # LLM integration
│   ├── speaker.py        # Speaker recognition
│   └── device.py         # Device management
├── models/
│   ├── asr.py            # ASR (Automatic Speech Recognition)
│   ├── tts.py            # TTS (Text-to-Speech)
│   ├── llm.py            # LLM integration
│   └── speaker_id.py     # Speaker identification
├── services/
│   ├── opus_codec.py     # OPUS audio codec
│   ├── websocket_handler.py
│   ├── mqtt_handler.py
│   └── device_mcp.py     # Device MCP
├── utils/
│   ├── logger.py
│   ├── helpers.py
│   └── validators.py
├── tests/
│   ├── test_audio.py
│   ├── test_llm.py
│   └── test_speaker.py
└── migrations/           # Database migrations
```

## Setup

### 1. Create Virtual Environment

```bash
cd backend
python -m venv venv

# Activate
# On macOS/Linux:
source venv/bin/activate
# On Windows:
venv\Scripts\activate
```

### 2. Install Dependencies

```bash
pip install -r requirements.txt
```

### 3. Configure Environment

```bash
cp .env.example .env
```

Edit `.env`:
```env
DEBUG=True
FLASK_ENV=development
LLM_API_KEY=your_key_here
ASR_MODEL=whisper
TTS_MODEL=glow-tts
```

### 4. Run Server

```bash
python app.py
```

Server runs at `http://localhost:5000`

## API Endpoints

### Audio Processing

#### Transcribe Audio (ASR)
```
POST /api/audio/transcribe
Content-Type: audio/wav or audio/opus

Response:
{
  "status": "success",
  "text": "transcribed text",
  "confidence": 0.95
}
```

#### Synthesize Speech (TTS)
```
POST /api/tts/synthesize
Content-Type: application/json

Body:
{
  "text": "Hello world",
  "language": "en",
  "voice": "default"
}

Response:
{
  "status": "success",
  "audio": "base64_encoded_audio",
  "format": "opus"
}
```

### LLM Integration

#### Chat Endpoint
```
POST /api/llm/chat
Content-Type: application/json

Body:
{
  "message": "user message",
  "context": [],
  "language": "en"
}

Response:
{
  "status": "success",
  "response": "LLM response",
  "tokens_used": 150
}
```

### Speaker Recognition

#### Identify Speaker
```
POST /api/speaker/identify
Content-Type: audio/wav or audio/opus

Response:
{
  "status": "success",
  "speaker_id": "speaker_001",
  "confidence": 0.92,
  "name": "John Doe"
}
```

## Implementation Guide

### ASR (Automatic Speech Recognition)

```python
# models/asr.py
from openai import OpenAI
import io

class ASRService:
    def __init__(self):
        self.client = OpenAI(api_key=os.getenv('OPENAI_API_KEY'))
    
    def transcribe(self, audio_data, language='en'):
        """Transcribe audio using Whisper"""
        audio_file = io.BytesIO(audio_data)
        
        transcript = self.client.audio.transcriptions.create(
            model="whisper-1",
            file=audio_file,
            language=language
        )
        
        return transcript.text
```

### TTS (Text-to-Speech)

```python
# models/tts.py
from edge_tts import Communicate
import asyncio

class TTSService:
    async def synthesize(self, text, language='en', voice='en-US-AriaNeural'):
        """Synthesize text to speech"""
        communicate = Communicate(text=text, voice=voice)
        audio_data = b''
        
        async for chunk in communicate.stream():
            if chunk['type'] == 'audio':
                audio_data += chunk['data']
        
        return audio_data
```

### LLM Integration

```python
# models/llm.py
import openai

class LLMService:
    def __init__(self):
        openai.api_key = os.getenv('OPENAI_API_KEY')
    
    def chat(self, message, context=None, language='en'):
        """Get LLM response"""
        messages = []
        
        if context:
            messages.extend(context)
        
        messages.append({
            "role": "user",
            "content": message
        })
        
        response = openai.ChatCompletion.create(
            model="gpt-3.5-turbo",
            messages=messages,
            temperature=0.7,
            max_tokens=150
        )
        
        return response.choices[0].message.content
```

### Speaker Recognition

```python
# models/speaker_id.py
import numpy as np
from sklearn.metrics.pairwise import cosine_similarity

class SpeakerRecognitionService:
    def __init__(self):
        self.model = self.load_model()  # Load pre-trained model
        self.speaker_embeddings = {}  # Store speaker embeddings
    
    def identify_speaker(self, audio_data):
        """Identify speaker from audio"""
        embedding = self.extract_embedding(audio_data)
        
        similarities = {}
        for speaker_id, stored_embedding in self.speaker_embeddings.items():
            sim = cosine_similarity([embedding], [stored_embedding])[0][0]
            similarities[speaker_id] = sim
        
        best_speaker = max(similarities, key=similarities.get)
        confidence = similarities[best_speaker]
        
        return {
            'speaker_id': best_speaker,
            'confidence': float(confidence)
        }
    
    def extract_embedding(self, audio_data):
        """Extract speaker embedding from audio"""
        # Process audio and extract features
        features = self.extract_mfcc(audio_data)
        embedding = self.model.predict(features)
        return embedding
```

## Testing

### Unit Tests

```bash
pytest tests/
```

### Example Test

```python
# tests/test_audio.py
import pytest
from backend.models.asr import ASRService

def test_transcribe():
    asr = ASRService()
    audio_data = b'test_audio_data'
    
    result = asr.transcribe(audio_data)
    
    assert isinstance(result, str)
    assert len(result) > 0
```

## Deployment

### Production Server

Use Gunicorn:
```bash
gunicorn -w 4 -b 0.0.0.0:5000 app:app
```

With Nginx reverse proxy:
```nginx
server {
    listen 80;
    server_name your_domain.com;
    
    location / {
        proxy_pass http://127.0.0.1:5000;
        proxy_set_header Host $host;
        proxy_set_header X-Real-IP $remote_addr;
    }
}
```

### Docker

```bash
docker build -t esp32-backend .
docker run -p 5000:5000 esp32-backend
```

## Performance Optimization

1. **Caching**
   - Cache LLM responses
   - Store speaker embeddings
   - Cache audio features

2. **Async Processing**
   - Use async for I/O operations
   - Process audio chunks asynchronously
   - Queue long-running tasks

3. **Resource Management**
   - Monitor memory usage
   - Limit concurrent connections
   - Implement rate limiting

## Troubleshooting

### Common Issues

1. **API Key Errors**
   - Verify API keys in `.env`
   - Check API quotas
   - Validate key permissions

2. **Audio Processing Issues**
   - Check audio format
   - Verify sample rate
   - Monitor audio quality

3. **LLM Timeout**
   - Increase timeout values
   - Reduce prompt complexity
   - Check network connection

## References

- [Flask Documentation](https://flask.palletsprojects.com/)
- [OpenAI API](https://platform.openai.com/docs/)
- [librosa Documentation](https://librosa.org/)
- [SciPy Audio](https://docs.scipy.org/doc/scipy/reference/io.wavfile.html)
