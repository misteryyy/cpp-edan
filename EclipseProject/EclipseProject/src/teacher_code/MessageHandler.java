package common;

/**
 * Message handler is the low-level message handler for messages to/from the
 * server. It handles transmission/reception of low-level message units.
 */

public class MessageHandler {
	private Connection conn; // the connection
	private Logger logWindow; // the log window

	/**
	 * Create a message handler.
	 * 
	 * @param conn
	 *            The connection to use messages
	 */
	public MessageHandler(Connection conn) {
		this.conn = conn;
	}

	/**
	 * Set the log window to use.
	 * 
	 * @param logWindow
	 *            The log window
	 */
	public void setLogWindow(Logger logWindow) {
		this.logWindow = logWindow;
	}

	private void sendByte(int code) throws ConnectionClosedException {
		try {
			conn.write((char) code);
		} catch (java.io.IOException e) {
			throw new ConnectionClosedException();
		}
	}

	/**
	 * Transmit a code (a constant from the Protocol class).
	 * 
	 * @param code
	 *            The code to transmit.
	 * @throws ConnectionClosedException
	 *             If the server died
	 */
	public void sendCode(int code) throws ConnectionClosedException {
		sendByte(code);
		logWindow.logCode(code);
	}

	/**
	 * Transmit an int value, according to the protocol.
	 * 
	 * @param value
	 *            The value to transmit
	 * @throws ConnectionClosedException
	 *             If the server died
	 */
	public void sendInt(int value) throws ConnectionClosedException {
		sendByte((value >> 24) & 0xFF);
		logWindow.logByte((value >> 24) & 0xFF);
		sendByte((value >> 16) & 0xFF);
		logWindow.logByte((value >> 16) & 0xFF);
		sendByte((value >> 8) & 0xFF);
		logWindow.logByte((value >> 8) & 0xFF);
		sendByte(value & 0xFF);
		logWindow.logByte(value & 0xFF);
	}

	/**
	 * Transmit an int parameter, according to the protocol.
	 * 
	 * @param value
	 *            The parameter to transmit
	 * @throws ConnectionClosedException
	 *             If the server died
	 */
	public void sendIntParameter(int param) throws ConnectionClosedException {
		sendCode(Protocol.PAR_NUM);
		sendInt(param);
	}

	/**
	 * Transmit a string parameter, according to the protocol.
	 * 
	 * @param value
	 *            The parameter to transmit
	 * @throws ConnectionClosedException
	 *             If the server died
	 */
	public void sendStringParameter(String param)
			throws ConnectionClosedException {
		sendCode(Protocol.PAR_STRING);
		sendInt(param.length());
		for (int i = 0; i < param.length(); i++) {
			sendByte(param.charAt(i));
			logWindow.logChar(param.charAt(i));
		}
	}

	private int recvByte() throws ConnectionClosedException {
		int code = conn.read();
		if (code == Connection.CONNECTION_CLOSED) {
			throw new ConnectionClosedException();
		}
		return code;
	}

	/**
	 * Receive a command code or an error code from the server.
	 * 
	 * @return The code
	 * @throws ConnectionClosedException
	 *             If the server died
	 */
	public int recvCode() throws ConnectionClosedException {
		int code = recvByte();
		logWindow.logCode(code);
		return code;
	}

	/**
	 * Receive an int value from the server.
	 * 
	 * @return The int value
	 * @throws ConnectionClosedException
	 *             If the server died
	 */
	public int recvInt() throws ConnectionClosedException {
		int b1 = recvByte();
		logWindow.logByte(b1);
		int b2 = recvByte();
		logWindow.logByte(b2);
		int b3 = recvByte();
		logWindow.logByte(b3);
		int b4 = recvByte();
		logWindow.logByte(b4);

		return b1 << 24 | b2 << 16 | b3 << 8 | b4;
	}

	/**
	 * Receive an int parameter from the server.
	 * 
	 * @return The parameter value
	 * @throws ConnectionClosedException
	 *             If the server died
	 */
	public int recvIntParameter() throws ConnectionClosedException {
		int code = recvCode();
		if (code != Protocol.PAR_NUM) {
			throw new ProtocolViolationException("Receive numeric parameter",
					Protocol.PAR_NUM, code);
		}
		return recvInt();
	}

	/**
	 * Receive a string parameter from the server.
	 * 
	 * @return The parameter value
	 * @throws ConnectionClosedException
	 *             If the server died
	 */
	public String recvStringParameter() throws ConnectionClosedException {
		int code = recvCode();
		if (code != Protocol.PAR_STRING) {
			throw new ProtocolViolationException("Receive string parameter",
					Protocol.PAR_STRING, code);
		}
		int n = recvInt();
		if (n < 0) {
			throw new ProtocolViolationException("Receive string parameter",
					"Number of characters < 0");
		}
		StringBuffer result = new StringBuffer(n);
		for (int i = 1; i <= n; i++) {
			char ch = (char) conn.read();
			result.append(ch);
			logWindow.logChar(ch);
		}
		return result.toString();
	}
}
